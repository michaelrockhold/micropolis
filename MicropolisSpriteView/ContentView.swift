//
//  ContentView.swift
//  MicropolisSpriteView
//
//  Created by Michael Rockhold on 3/16/23.
//

import SwiftUI
import SpriteKit
import MicropolisKit

class GameScene: SKScene {
}

struct ContentView: View {
    @ObservedObject var cityObservable : CityObservable
    @State private var dateString: String = ""
    @State private var statusMessage: String = ""
    @State private var isImportantMessage = false

    private var gameScene: GameScene
    private var dateFormatter: DateFormatter
    
    init(cityObservable: CityObservable) {
        
        self.cityObservable = cityObservable
        
        let gameScene = GameScene()
        gameScene.size = CGSize(width: 1920, height: 1600)
        gameScene.anchorPoint = CGPoint(x: 0.5, y: 0.5)
        gameScene.scaleMode = .fill
        cityObservable.tileMapNode.removeFromParent()
        gameScene.addChild(cityObservable.tileMapNode)

        self.gameScene = gameScene
        
        self.dateFormatter = DateFormatter();
        dateFormatter.timeStyle = .none
        dateFormatter.dateStyle = .medium
        
        Task.detached {
            try! await Task.sleep(for: Duration.seconds(1))
            cityObservable.run()
        }
    }
    
    var body: some View {
        HStack {
            Text(statusMessage); Spacer(); Text(dateString)
        }.onChange(of: cityObservable.cityDate) { newValue in
            dateString = self.dateFormatter.string(from:newValue)
        }.onChange(of: cityObservable.message) { newValue in
            if cityObservable.isImportantMessage {
                isImportantMessage = true
                cityObservable.pause()
            }
            statusMessage = newValue
        }
        .alert(
            "Important Message",
            isPresented: $isImportantMessage,
            presenting: cityObservable.message
        ) { details in
            Button("Whatever") {
                cityObservable.go()
            }
        } message: { details in
            Text(statusMessage)
        }
        

        ScrollView(Axis.Set(arrayLiteral: .horizontal, .vertical), showsIndicators: true) {
            VStack {
                
                SpriteView(scene: gameScene)
                    .frame(width: 1920, height: 1600)
                    .ignoresSafeArea()
            }
        }
    }
}

private let itemFormatter: DateFormatter = {
    let formatter = DateFormatter()
    formatter.dateStyle = .short
    formatter.timeStyle = .medium
    return formatter
}()

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView(cityObservable: CityObservable(cityModel: CityModel(), worldWidth: 32, worldHeight: 32))
    }
}
