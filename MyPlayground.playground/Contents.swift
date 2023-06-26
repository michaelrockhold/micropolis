import Foundation
import SwiftUI
import PlaygroundSupport

struct CityData: Codable {
    let name: String
    let title: String
    let description: String
    let cityTime: Int
    let funds: Int
    let cityData: Data
}

let markdown = """
Hello, *world*
What are the _three_ items?
See the *latest* news at [our website](https://example.com).
 - How ~does~goes it?
 - Fine, I hope
 - Line 3
"""

let markdown2 = "Hello, *world*\nWhat are the _three_ items?\nSee the *latest* news at [our website](https://example.com)."

let markdownData = Data(markdown.utf8)

let aString = try! AttributedString(markdown: markdown)

let anotherString = try! NSAttributedString(markdown: "",
                                       options: AttributedString.MarkdownParsingOptions())

//PlaygroundPage.current.setLiveView(
//    VStack(alignment: .leading) {
//        Text("""
//            Hello, **world**
//            What are the _three_ items?
//            See the *latest* news at [our website](https://example.com).
//             - How ~does~goes it?
//             - Fine, I hope
//             - Line 3
//            """)
//    }
//)

var textField = NSTextField(labelWithAttributedString: anotherString)
textField.cell!.usesSingleLineMode = false
textField.cell!.wraps = true
//textField.maximumNumberOfLines = 100
textField.setFrameSize(NSSize(width: 100, height: 500))

PlaygroundPage.current.liveView = textField
//
//let rawData = Data(contentsOf: URL(filePath: "/Users/mr/Developer/Projects/micropolis/MicropolisKit/Resources/SCScenarios/BOSTON.scscenario/city.cty"))
//
//let cityData = CityData(name: "Seattle",
//                        title: "Queen City of the Pacific Northwest",
//                        description: "A lot of action",
//                        cityTime: 123,
//                        funds: 10000,
//                        cityData: rawData)
//
//var propertyListDecoder = PropertyListDecoder()
//var propertyListEncoder = PropertyListEncoder()
//propertyListEncoder.outputFormat = .xml
//
//let data = try! propertyListEncoder.encode(cityData)
//
////print(data.base64EncodedString(options: .endLineWithCarriageReturn))
////print(String(data: data, encoding: .utf8)!)
//
//let city2 = propertyListDecoder.decode(CityData.self, from: data)
//
//let pliststr = """
//<?xml version="1.0" encoding="UTF-8"?>
//<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
//<plist version="1.0">
//<dict>
//    <key>cityTime</key>
//    <integer>123</integer>
//    <key>description</key>
//    <string>A lot of action</string>
//    <key>funds</key>
//    <integer>10000</integer>
//    <key>name</key>
//    <string>Seattle</string>
//    <key>title</key>
//    <string>Queen City of the Pacific Northwest</string>
//    <key>cityData</key>
//    <data>WlpaWlpaWlpaWlpa</data>
//</dict>
//</plist>
//"""
//
//let city3 = propertyListDecoder.decode(CityData.self, from: pliststr.data(using: .utf8)!)
