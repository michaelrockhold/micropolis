//
//  CityModel.swift
//  MicropolisKit
//
//  Created by Michael Rockhold on 2/18/23.
//

import Foundation
import Combine
import CoreGraphics


struct Notice {
    let id: Int
    let color: CGColor
    let title: String
    let description: String
    let sprite: SpriteType?
}

extension Notice: Codable {
    init(from decoder: Decoder) throws {
        
        self.init(id: 0, color: CGColor(red: 0.5, green: 0.5, blue: 0.5, alpha: 1.0), title: "title", description: "description", sprite: nil)
    }
    
    public func encode(to encoder: Encoder) throws {
        
    }
}

public class CityModel {
        
    public static var bundle: Bundle {
        Bundle(for: NSClassFromString("MicropolisKit.CityModel")!)
    }

    public static let WORLD_W = 120
    public static let WORLD_H = 100
        
    public typealias TileCoord = ( x: Int, y: Int )

    public enum Interest: String {
        public typealias RawValue = String
        
        case Date = "date"
        case Message = "message"
        case City = "city"
        case CityName = "cityName"
        case Demand = "demand"
        case Evaluation = "evaluation"
        case Funds = "funds"
        case TaxRate = "taxRate"
        case Budget = "budget"
        case GameLevel = "gameLevel"
        case GameMode = "gameMode"
        case History = "history"
        case Load = "load"
        case Map = "map"
        case Options = "options"
        case Passes = "passes"
        case Paused = "paused"
        case Speed = "speed"
        case Tick = "tick"
        case Editor = "editor"
    }
    
    public enum SoundChannel {
        case ui
        case city
    }
    
    public enum Event {
        case Nothing
        case PlayNewCity
        case StartLoad
        case StartScenario(Int /* scenario number */)
        case StartEarthquake(Int /* strength */)
        case Update(Interest)
        case UpdateTime(Int /* cityTime */)
        case MakeSound(String /* soundName */, SoundChannel, TileCoord)
        case SimRobots
        case SimulateChurch(Int /* x */, Int /* y */, Int /* churchNumber */)
        case SendMessage(MessageNumber /* mesgNum */, Int /* x */, Int /* y */, Bool /* picture */, Bool /* important */)
        case AutoGoto(Int /* x */, Int /* y */, String /* msg */)
        case LoseGame
        case WinGame
        case ShowBudgetAndWait
        case DidLoadScenario
        case DidLoadCity
        case DidntLoadCity(String /* msg */)
        case SaveCityAs
        case DidSaveCity
        case DidntSaveCity(String /* msg */)
        case NewGame
        case ShowZoneStatus(Int /* tileCategory */, Int /* s0 */, Int /* s1 */, Int /* s2 */, Int /* s3 */, Int /* s4 */, Int /* x */, Int /* y */)
        case DidTool(String /* name */, Int /* x */, Int /* y */)
        case DidGenerateMap
        case Tile(Int, Int, Int) // column, row, tileIdx
        
        case CreatedSprite(Int) // sprite ID
        case DestroyedSprite(Int) // sprite ID
        case UpdatedSprite(Int) // sprite ID
        
    }
            
    private let cityActor: CityActor
    private let cityEventSender = CityEventSender()
    public let cityEventPublisher: AnyPublisher<Event, Never>

    public init() {
        cityEventPublisher = cityEventSender.cityEventPublisher
        cityActor = CityActor(wrapper: MicropolisWrapper(cityEventSender))
        
        
    }
    
    public func run(scenario: SCScenario) {
        Task.detached {
            self.load(scenario: scenario)
        }
    }
    
    
    private var cancellableClockTimer: Cancellable? = nil
    private var mapUpdateTimer: Cancellable? = nil

    private let refreshInterval = 0.01
    public func startClock() {
        
        stopClock()
        
        cityActor.micropolisWrapper.setSimSpeed(3)

        cancellableClockTimer = DispatchQueue
            .global(qos: .utility)
            .schedule(after: DispatchQueue.SchedulerTimeType(.now()),
                      interval: .seconds(refreshInterval),
                      tolerance: .seconds(refreshInterval / 5)) { [weak self] in
            guard let self else { return }
                Task.detached { self.tick() }
        }
        
        mapUpdateTimer = DispatchQueue
            .global(qos: .utility)
            .schedule(after: DispatchQueue.SchedulerTimeType(.now()),
                      interval: .seconds(1.0),
                      tolerance: .seconds(1.0 / 5)) { [weak self] in
            guard let self else { return }
                Task.detached { self.updateMap() }
        }
    }
    
    public func stopClock() {
        mapUpdateTimer?.cancel()
        mapUpdateTimer = nil
        cancellableClockTimer?.cancel()
        cancellableClockTimer = nil
    }

    public func generateSomeCity(with seed: Int) {
        cityActor.micropolisWrapper.generateSomeCity(Int32(seed))
    }
    
    public func load(scenario: SCScenario) {
        
        scenario.name.withCString { name in
            scenario.title.withCString { title in
                scenario.description.withCString { description in
                    scenario.cityData.withUnsafeBytes { data in
                        let scenarioParams = newScenario(
                            /* name */ UnsafeMutablePointer<CChar>(mutating: name),
                           /* title */ UnsafeMutablePointer<CChar>(mutating: title),
                           /* description */ UnsafeMutablePointer<CChar>(mutating: description),
                                       data.baseAddress, // throw away our knowledge of the size of the buffer to placate C/C++ "unsigned char *"
                           /* cityTime */ Int32(scenario.cityTime),
                           /* funds */ Int32(scenario.funds),
                           /* crisisType */ scenario.crisisType,
                           /* crisisTimeMode */ scenario.crisisTimeMode,
                           /* crisisTime */ Int32(scenario.crisisTime),
                           /* disasterWait */ Int32(scenario.disasterWait),
                           /* scoreWait */ Int32(scenario.scoreWait),
                           /* winCriterion */ scenario.winCriterion,
                           /* winCriterionArg */ Int32(scenario.winCriterionArg),
                           /* hasFoghorns */ scenario.hasFoghorns)
                        
                        //withUnsafeMutablePointer(to: scenarioParams) {
                        cityActor.micropolisWrapper.load(scenarioParams)
                        //}
                    }
                }
            }
        }
    }

    private func tick() {
        cityActor.micropolisWrapper.simTick()
    }
    
    private func updateMap() {
        cityActor.micropolisWrapper.generateMapUpdates()
    }

}

public actor CityActor {
    public let micropolisWrapper: MicropolisWrapper
    public init(wrapper: MicropolisWrapper) {
        self.micropolisWrapper = wrapper
    }
}
