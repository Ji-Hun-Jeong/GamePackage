#include "pch.h"
#include "WzCharacterLoader.h"

/* 
{
  "dir": {
	"@name": "00002000.img",
	"dir": [
	  {
		"@name": "info",
		"string": [
		  { "@name": "islot", "@value": "Bd" },
		  { "@name": "vslot", "@value": "Bd" }
		],
		"int32": { "@name": "cash", "@value": "0" }
	  },
	  {
		"@name": "walk1",
		"dir": [
		  {
			"@name": "0",
			"png": [
			  {
				"@name": "body",
				"@value": "iVBOR... (생략)",
				"vector": { "@name": "origin", "@value": "19, 32" },
				"dir": {
				  "@name": "map",
				  "vector": [
					{ "@name": "neck", "@value": "-4, -32" },
					{ "@name": "navel", "@value": "-6, -20" }
				  ]
				},
				"string": [
				  { "@name": "z", "@value": "body" },
				  { "@name": "group", "@value": "skin" },
				  { "@name": "_outlink", "@value": "Character/_Canvas/00002000.img/walk1/0/body" }
				]
			  },
			  {
				"@name": "arm",
				"@value": "iVBOR... (생략)",
				"vector": { "@name": "origin", "@value": "6, 8" },
				"dir": {
				  "@name": "map",
				  "vector": [
					{ "@name": "navel", "@value": "-12, 2" },
					{ "@name": "hand", "@value": "1, 5" }
				  ]
				},
				"string": [
				  { "@name": "z", "@value": "arm" },
				  { "@name": "group", "@value": "skin" },
				  { "@name": "_outlink", "@value": "Character/_Canvas/00002000.img/walk1/0/arm" }
				]
			  }
			],
			"int16": { "@name": "face", "@value": "1" },
			"int32": { "@name": "delay", "@value": "180" }
		  },
		  {
			"@name": "1",
			"png": [
			  {
				"@name": "body",
				"@value": "iVBOR... (생략)",
				"vector": { "@name": "origin", "@value": "16, 32" }
			  }
			],
			"int32": { "@name": "delay", "@value": "180" }
		  }
		]
	  }
	]
  }
}
*/

CWzCharacterLoader::CWzCharacterLoader()
{
}