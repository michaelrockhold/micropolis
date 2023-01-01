//
//  micropolis_engine_tests.swift
//  micropolis_engine_tests
//
//  Created by Michael Rockhold on 12/31/22.
//

import XCTest
import micropolis_engine

class TestCallbackDelegate: NSObject, micropolis_engine.MicropolisDelegate {
    
    var lastCallbackName: String? = nil
    
    @objc func micropolisWrapper(_ wrapper: MicropolisWrapper!,
                           callback name: String!,
                           args arglist: [Any]) {
        
        lastCallbackName = name
    }
    
    
}
                                
                                
final class micropolis_engine_tests: XCTestCase {

    var engine: micropolis_engine.MicropolisWrapper!
    var callbackDelegate: TestCallbackDelegate!
    
    override func setUpWithError() throws {
        // Put setup code here. This method is called before the invocation of each test method in the class.
        callbackDelegate = TestCallbackDelegate();
        engine = micropolis_engine.MicropolisWrapper(callbackDelegate:callbackDelegate)
    }

    override func tearDownWithError() throws {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        engine = nil
        callbackDelegate = nil
    }

    func testExample() throws {
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        // Any test you write for XCTest can be annotated as throws and async.
        // Mark your test throws to produce an unexpected failure when your test encounters an uncaught error.
        // Mark your test async to allow awaiting for asynchronous code to complete. Check the results with assertions afterwards.
        XCTAssertEqual(engine.getMicropolisVersion(), "5.0")
    }

    func testPerformanceExample() throws {
        // This is an example of a performance test case.
        measure {
            // Put the code you want to measure the time of here.
        }
    }

}
