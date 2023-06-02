//
//  AppDelegate.swift
//  MicropolisSprite
//
//  Created by Michael Rockhold on 2/23/23.
//


import Cocoa
import MicropolisKit
import Combine

@main
class AppDelegate: NSObject, NSApplicationDelegate {
    
    public let cityModel: MicropolisKit.CityModel

    override init() {
        cityModel = MicropolisKit.CityModel()
    }
    
//    func applicationDidFinishLaunching(_ aNotification: Notification) {
//        self.cityModel.run()
//    }
    
    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
    
}
