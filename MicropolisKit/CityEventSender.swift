//
//  CityEventSender.swift
//  MicropolisKit
//
//  Created by Michael Rockhold on 3/31/23.
//

import Foundation
import Combine

@objc public class CityEventSender: NSObject, MicropolisOutgoing {    
    
    private let eventSubject: any Subject<CityModel.Event, Never> = CurrentValueSubject<CityModel.Event, Never>(CityModel.Event.Nothing)
    
    private func tileCoord(x: Int32, y: Int32) -> CityModel.TileCoord {
        return CityModel.TileCoord(x: Int(x), y: Int(y))
    }

    public var cityEventPublisher: AnyPublisher<CityModel.Event, Never> {
        eventSubject.eraseToAnyPublisher()
    }
    public func doPlayNewCity() {
        eventSubject.send(CityModel.Event.PlayNewCity)
    }
    public func doEarthquake(withStrength strength: Int32) {
        eventSubject.send(CityModel.Event.StartEarthquake(Int(strength)))
    }
    public func makeSound(_ sound: String!, forChannel channel: String!, atX x: Int32, andY y: Int32) {
        let chnl = channel == "city" ? CityModel.SoundChannel.city : CityModel.SoundChannel.ui
        eventSubject.send(CityModel.Event.MakeSound(sound, chnl, tileCoord(x: x, y: y)))
    }
    public func doRobots() {
        eventSubject.send(CityModel.Event.SimRobots)
    }
    public func doSimulateChurch(_ churchNumber: Int32, atX x: Int32, andY y: Int32) {
        eventSubject.send(CityModel.Event.SimulateChurch(Int(x), Int(y), Int(churchNumber)))
    }
    public func sendMessage(withMesgNum mesgNum: MessageNumber, atX x: Int32, andY y: Int32, withPicture picture: Bool, andImportance important: Bool) {
        eventSubject.send(CityModel.Event.SendMessage(mesgNum, Int(x), Int(y), picture, important))
    }
    public func doAutoGotoX(_ x: Int32, andY y: Int32, withMsg msg: String!) {
        eventSubject.send(CityModel.Event.AutoGoto(Int(x), Int(y), msg))
    }
    public func doLoseGame() {
        eventSubject.send(CityModel.Event.LoseGame)
    }
    public func doWinGame() {
        eventSubject.send(CityModel.Event.WinGame)
    }
    public func doSaveCityAs() {
        eventSubject.send(CityModel.Event.SaveCityAs)
    }
    public func doNewGame() {
        eventSubject.send(CityModel.Event.NewGame)
    }
    public func doShowZoneStatus(forTileCategory tileCategory: Int32, s0: Int32, s1: Int32, s2: Int32, s3: Int32, s4: Int32, atX x: Int32, andY y: Int32) {
        eventSubject.send(CityModel.Event.ShowZoneStatus(Int(tileCategory), Int(s0), Int(s1), Int(s2), Int(s3), Int(s4), Int(x), Int(y)))
    }
    public func didTool(withName name: String!, atX x: Int32, andY y: Int32) {
        eventSubject.send(CityModel.Event.DidTool(name, Int(x), Int(y)))
    }
    public func updateTarget(_ target: String!) {
        guard let interestStr = target else { fatalError("calling updateTarget without an interest") }
        guard let interest = CityModel.Interest(rawValue: interestStr) else {
            fatalError("unexpected Update interest \(interestStr)")
        }
        eventSubject.send(CityModel.Event.Update(interest))

//        switch interest {
//        case .Map:
//            updateMap()
//        default:
//            eventSubject.send(CityModel.Event.Update(interest))
//        }
    }
    public func updateTime(_ cityTime: Int32) {
        eventSubject.send(CityModel.Event.UpdateTime(Int(cityTime)));
    }
    public func showBudgetAndWait() {
        eventSubject.send(CityModel.Event.ShowBudgetAndWait)
    }
    public func didLoadScenario() {
        eventSubject.send(CityModel.Event.DidLoadScenario)
    }
    public func didLoadCity() {
        eventSubject.send(CityModel.Event.DidLoadCity)
    }
    public func didntLoadCity(withMsg msg: String) {
        eventSubject.send(CityModel.Event.DidntLoadCity(msg))
    }
    public func saveCityAs() {
        eventSubject.send(CityModel.Event.SaveCityAs)
    }
    public func didSaveCity() {
        eventSubject.send(CityModel.Event.DidSaveCity)
    }
    public func didntSaveCity(withMsg msg: String) {
        eventSubject.send(CityModel.Event.DidntSaveCity(msg))
    }
    public func newGame() {
        eventSubject.send(CityModel.Event.NewGame)
    }
    
    public func didGenerateMap() {
        eventSubject.send(CityModel.Event.DidGenerateMap)
//        eventSubject.send(CityModel.Event.Update(.Tick))
//        eventSubject.send(CityModel.Event.Update(.Editor))
//        updateMap()
    }
    
    func updateMap() {
        //        eventSubject.send(CityModel.Event.Map(self.getMapBuffer()!))
    }
    
    public func updateTileAt(x: Int32, andY y: Int32, tileIdx t: Int32) {
        eventSubject.send(CityModel.Event.Tile(Int(x), Int(y), Int(t)))
    }
    
    public func didCreateSprite(_ spriteID: Int32) {
        eventSubject.send(CityModel.Event.CreatedSprite(Int(spriteID)));
    }
    
    public func willDestroySprite(_ spriteID: Int32) {
        eventSubject.send(CityModel.Event.DestroyedSprite(Int(spriteID)));
    }

    public func didUpdateSprite(_ spriteID: Int32) {
        eventSubject.send(CityModel.Event.UpdatedSprite(Int(spriteID)));
    }
}
