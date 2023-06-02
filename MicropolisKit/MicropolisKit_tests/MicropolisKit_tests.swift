//
//  MicropolisKit_tests.swift
//  MicropolisKit_tests
//
//  Created by Michael Rockhold on 12/31/22.
//

import XCTest
import MicropolisKit
                                
                                
final class MicropolisKit_tests: XCTestCase {

    var testApp: MicropolisKit.CityModel?
    
    override func setUpWithError() throws {
        // Put setup code here. This method is called before the invocation of each test method in the class.
        testApp = MicropolisKit.CityModel()
    }

    override func tearDownWithError() throws {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
//        delete testApp
    }

    func testExample() async throws {
                
        var event: CityModel.Event? = nil
        
        let subscription = await testApp?.eventPublisher.sink {
            event = $0
        }

        await testApp?.startClock()
        
        switch event {
        case .Update(let kind):
            XCTAssertEqual(kind, "speed")
        default:
            XCTFail("published event was not the expected \"speed\"")
        }
        
    }

//    func testPerformanceExample() throws {
//        // This is an example of a performance test case.
//        measure {
//            // Put the code you want to measure the time of here.
//        }
//    }

}
