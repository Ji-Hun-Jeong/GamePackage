//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <fstream>
//#include <cassert>
//#include "xml2json.hpp"
//
//// RapidJSON 관련 추가 헤더
//#include "rapidjson/document.h"
//#include "rapidjson/filewritestream.h"
//#include "rapidjson/writer.h"
//
//int main()
//{
//    // 2. 파일 열기 (쓰기 모드)
//    FILE* File = fopen("E:/source/repos/GamePackage/Game/resources/data/Skill/Skill.2411.img.json", "w");
//    if (File == nullptr) assert(0);
//
//    // 1. XML 읽기 및 JSON 문자열 변환
//    rapidxml::file<> xmlFile("C:/Users/Jeong/Downloads/Skill.2411.img.xml");
//    std::string jsonResult = xml2json(xmlFile.data());
//
//    // 3. JSON 문자열을 다시 Document로 파싱 (쓰기 위해 필요)
//    rapidjson::Document doc;
//    doc.Parse(jsonResult.c_str());
//
//    // 4. FileWriteStream과 Writer 연결
//    const size_t BufferSize = 1024;
//    char Buffer[BufferSize];
//    rapidjson::FileWriteStream FileWriter(File, Buffer, BufferSize);
//
//    // Writer를 사용하여 doc의 내용을 FileWriter에 씁니다.
//    rapidjson::Writer<rapidjson::FileWriteStream> writer(FileWriter);
//    doc.Accept(writer);
//
//    // 5. 정리
//    fclose(File);
//    std::cout << "JSON 파일 저장 성공!" << std::endl;
//
//    return 0;
//}

//"E:/source/repos/GamePackage/Game/resources/data/Skill/Skill.2411.img.json", "w"

// C:/Users/Jeong/Downloads/Skill.2411.img.xml


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "rapidxml/rapidxml.hpp"
#include "nlohmann/json.hpp"

using namespace rapidxml;
// 상단 선언부 변경
// using json = nlohmann::json; // 기존
using json = nlohmann::ordered_json; // 순서 보존용 타입으로 변경

// 나머지 변환 로직(ConvertNode)은 그대로 두어도 
// j["key"] = value; 할 때 입력한 순서가 기억됩니다.

json ConvertNode(xml_node<>* node) {
	json j = json::object();

	// 자식 노드 순회
	for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
		xml_attribute<>* nameAttr = child->first_attribute("name");
		xml_attribute<>* valueAttr = child->first_attribute("value");

		if (nameAttr) {
			std::string key = nameAttr->value();

			// 1. 자식이 없고 value만 있는 노드 (예: int32, string)
			// 바로 값을 할당해서 계층을 줄임
			if (child->first_node() == nullptr && valueAttr) {
				j[key] = valueAttr->value();
			}
			// 2. 자식이 있는 노드 (예: dir, canvas)
			else {
				j[key] = ConvertNode(child);
				// 태그 정보가 필요하다면 별도 저장 (선택 사항)
				// j[key]["_type"] = child->name(); 
			}
		}
	}

	// 만약 현재 노드 자체가 value를 가지고 있다면 (png 데이터 등)
	xml_attribute<>* selfValue = node->first_attribute("value");
	if (selfValue && j.empty()) {
		return selfValue->value();
	}

	return j;
}

int main() {
	std::string FileName = "Skill.2412.img";
	// 1. XML 파일 읽기
	std::ifstream file("C:/Users/user/Downloads/" + FileName + ".xml");
	if (!file.is_open()) return -1;

	std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// 2. RapidXML 파싱
	xml_document<> doc;
	doc.parse<0>(&buffer[0]);

	// 3. 첫 번째 노드(보통 <dir name="000.img">)부터 변환 시작
	xml_node<>* root = doc.first_node();
	if (!root) return -1;

	json result = ConvertNode(root);

	// 4. JSON 파일 저장
	std::ofstream outFile("C:/Users/user/source/repos/GamePackage/Game/resources/data/Skill/" + FileName + ".json");
	outFile << result.dump(4);

	std::cout << "Success: 2411.img.json created." << std::endl;

	return 0;
}