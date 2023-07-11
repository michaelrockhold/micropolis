//
//  MicropolisSpriteViewApp.swift
//  MicropolisSpriteView
//
//  Created by Michael Rockhold on 3/16/23.
//

import SwiftUI
import MicropolisKit
import SpriteKit
import Combine
import CoreGraphics

@main
struct MicropolisSpriteViewApp: App {
    let persistenceController = PersistenceController.shared
    var cityModel: MicropolisKit.CityModel { get { return CityModel()} }
    var cityObservable: CityObservable { get {
        let co = CityObservable(cityModel: cityModel,
                                worldWidth: CityModel.WORLD_W,
                                worldHeight: CityModel.WORLD_H)
        return co
    }}

    var body: some Scene {
        WindowGroup {
            ContentView(cityObservable: cityObservable)
        }
    }
}

class CityObservable: ObservableObject {
    
    @Published var cityDate: Date = Date()
    @Published var message: String = ""
    
    public let cityModel: CityModel!
    public let tileMapNode: SKTileMapNode
    public let tileGroups: [SKTileGroup]
    
    static let cityTimeUnitSeconds = Int64(31556952 / 48)
    private let referenceDate: Date
            
    private var cityEventSubscription: AnyCancellable? = nil
    private let scenarios: [SCScenario]
    
    
    public init(cityModel: CityModel, worldWidth: Int, worldHeight: Int) {
        
        self.cityModel = cityModel
        
        scenarios = MicropolisKit.SCScenario.loadScenarioResources()
        
        let calendar = Calendar(identifier: .gregorian)
        let dateCompenents = DateComponents( year: 1900, month: 1, day: 1)
        referenceDate = calendar.date(from: dateCompenents)!
        
        tileGroups = (0..<1000).map { imageIdx in
            let imageName = String(format:"micropolis_tile_%04d", imageIdx)
            let texture = SKTexture(imageNamed: imageName)
            let tileDefinition = SKTileDefinition(texture: texture)
            return SKTileGroup(tileDefinition: tileDefinition)
        }
        let tileSet = SKTileSet(tileGroups: tileGroups)
        tileMapNode = SKTileMapNode(tileSet: tileSet, columns: worldWidth, rows: worldHeight, tileSize: CGSize(width: 16, height: 16))
        tileMapNode.enableAutomapping = false
        
        tileMapNode.fill(with: tileGroups[0])
        
        cityEventSubscription = cityModel.cityEventPublisher
            .receive(on: RunLoop.main)
            .sink { (event: CityModel.Event) in self.handle(event: event) }
        
        cityModel.run(scenario: scenarios[0])
    }
    
    private func setTile(forColumn column: Int, row: Int, tileIdx: Int) {
        
        guard (0..<1000).contains(tileIdx) else { return }
        guard (0..<CityModel.WORLD_W).contains(column) else { return }
        guard (0..<CityModel.WORLD_H).contains(row) else { return }
        
        self.tileMapNode.setTileGroup(tileGroups[tileIdx], forColumn: column, row: row)
    }
    
    private func soundURL(_ soundName: String) -> URL? {
        // TODO base this off a loaded map file rather than this hard-coded map
        let soundMap = [
//            a
//            aaah
//            airport
//            beep
//            boing
//            bop
//            build
//            bulldozer
//            chalk
//            coal
//            com
//            computer
//            cuckoo
//            e
//            eraser
            "ExplosionHigh": "explosion-high",
            "ExplosionLow": "explosion-low",
            "Fire": "fire",
            "HeavyTraffic": "heavytraffic",
            "HonkHonkHigh": "honkhonk-high",
            "HonkHonkLow": "honkhonk-low",
            "HonkHonkMed": "honkhonk-med",
//            ignition
//            ind
//            monster
//            nuclear
//            o
//            oop
//            park
//            police
            "Quack": "quack",
            "QuackQuack": "quackquack",
//            query
//            rail
//            res
//            road
//            rumble
//            seaport
//            siren
//            skid
//            sorry
//            stadium
//            uhuh
//            whip
//            wire
//            woosh
//            zone
        ]
        var sndResource: String?
        if let sr = soundMap[soundName] {
            sndResource = sr
        } else {
            sndResource = soundMap["Quack"]!
        }
        return CityModel.bundle.url(forResource: sndResource, withExtension: "wav")
    }
    
    private func sendSound(_ soundName: String, coord: CityModel.TileCoord? = nil) {
        guard let soundURL = soundURL(soundName) else { return }
        let audioNode = SKAudioNode(url: soundURL)
        audioNode.autoplayLooped = false
        if let coord = coord {
            audioNode.isPositional = true
            audioNode.position = tileMapNode.centerOfTile(atColumn: coord.x, row: coord.y)
        }
        
        self.tileMapNode.addChild(audioNode)
        audioNode.run(SKAction.sequence([
            SKAction.play(),
            SKAction.wait(forDuration: 2.0),
            SKAction.removeFromParent()
        ]))
    }
        
    private func handle(event: CityModel.Event) {
        switch event {
        case .UpdateTime(let cityTime):
            cityDate = Date(timeInterval: TimeInterval(integerLiteral: Int64(cityTime) * Self.cityTimeUnitSeconds), since: referenceDate)
            
        case .MakeSound(let soundName, let channel, let tileCoord):
            switch channel {
            case .ui:
                sendSound(soundName)
            case .city:
                sendSound(soundName, coord: tileCoord)
            }
            
        case .SendMessage(let msgNum, let x, let y, let isPicture, let isImportant):
            if isImportant  {
                print("The next message is important")
            }
            message = NSLocalizedString("\(msgNum.rawValue)",
                                                  tableName: "Messages",
                                                  bundle:  CityModel.bundle,
                                                  value: "-error-",
                                                  comment: "msg")
            
        case .Update(let kind):
            switch kind {
            case .Map:
                print("MAP UPDATE")
            case .History:
                print("History Update")
            case .Tick:
                print("Tick update")
            case .CityName:
                print("City Update")
            default:
                print("UNHANDLED UPDATE \(kind)")
            }
            
        case .Tile(let column, let row, let tileIdx):
            self.setTile(forColumn: column, row: row, tileIdx: tileIdx)
            
        case .DidLoadScenario:
            cityModel.startClock()
            
        case .DidLoadCity:
            cityModel.startClock()
            
        case .DidGenerateMap:
            cityModel.startClock()
            
        default:
            break
        }
    }
}
