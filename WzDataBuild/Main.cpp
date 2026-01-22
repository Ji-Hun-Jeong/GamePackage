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
#include "rapidxml/rapidxml_utils.hpp"
#include "nlohmann/json.hpp" // nlohmann json 라이브러리 사용

using namespace rapidxml;
using json = nlohmann::ordered_json; // 순서 보존을 위해 ordered_json 사용

// 1. 핵심 커스텀 변환 함수 (재귀)
json ConvertCompressed(xml_node<>* node) {
    json j = json::object();

    for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        xml_attribute<>* nameAttr = child->first_attribute("name");
        xml_attribute<>* valueAttr = child->first_attribute("value");

        if (nameAttr) {
            std::string key = nameAttr->value();

            if (child->first_node() == nullptr && valueAttr) {
                j[key] = valueAttr->value();
            }
            else {
                j[key] = ConvertCompressed(child);

                // 부모 노드 자체가 값을 가지고 있다면 'value'라는 키로 저장
                if (valueAttr) {
                    j[key]["value"] = valueAttr->value();
                }
            }
        }
    }
    return j;
}

int main() {
    // 경로 설정
    std::string xmlPath = "C:/Users/Jeong/Downloads/Skill.2411.img.xml";
    std::string jsonPath = "E:/source/repos/GamePackage/Game/resources/data/Skill/Skill.2411.img.json";

    try {
        std::cout << "XML 읽는 중..." << std::endl;

        // 2. XML 파일 로드 (BOM 무시를 위해 포인터 조정)
        rapidxml::file<> xmlFile(xmlPath.c_str());
        char* xmlData = xmlFile.data();

        // UTF-8 BOM(0xEF, 0xBB, 0xBF) 체크 및 제거
        if (static_cast<unsigned char>(xmlData[0]) == 0xEF &&
            static_cast<unsigned char>(xmlData[1]) == 0xBB &&
            static_cast<unsigned char>(xmlData[2]) == 0xBF) {
            xmlData += 3;
        }

        xml_document<> doc;
        doc.parse<0>(xmlData);

        // 3. 변환 시작 (첫 번째 루트 노드부터)
        xml_node<>* root = doc.first_node();
        if (!root) return -1;

        std::cout << "JSON 변환 중..." << std::endl;
        json result;

        // 루트 노드의 name이 있으면 그것을 최상위 키로 사용
        xml_attribute<>* rootName = root->first_attribute("name");
        if (rootName) {
            result[rootName->value()] = ConvertCompressed(root);
        }
        else {
            result = ConvertCompressed(root);
        }

        // 4. JSON 파일 저장
        std::ofstream outFile(jsonPath);
        if (outFile.is_open()) {
            // 들여쓰기 4칸으로 저장 (Pretty Print)
            outFile << result.dump(4);
            outFile.close();
            std::cout << "성공: " << jsonPath << " 저장 완료!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "에러 발생: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}