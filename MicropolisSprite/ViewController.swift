//
//  ViewController.swift
//  MicropolisSprite
//
//  Created by Michael Rockhold on 2/23/23.
//

import Cocoa
import SpriteKit
import GameplayKit
import MicropolisKit
import Combine

class ViewController: NSViewController {
    
    @IBOutlet var spriteKitView: SKView?
    @IBOutlet var messageLabel: NSTextField?
    @IBOutlet var dateLabel: NSTextField?
    private let dateFormatter = DateFormatter()

    private var viewModel: ViewModel!
    private var dateCancellable: Cancellable?
    private var messageCancellable: Cancellable?
    private var soundCancellable: Cancellable?

    override func viewDidLoad() {
        super.viewDidLoad()
        
        guard let spriteKitView = self.spriteKitView else {
            fatalError("no spritekitview")
        }

        viewModel = ViewModel(cityModel: (NSApplication.shared.delegate as! AppDelegate).cityModel, worldWidth: CityModel.WORLD_W, worldHeight: CityModel.WORLD_H)
        
        dateFormatter.timeStyle = .none
        dateFormatter.dateStyle = .medium

        dateCancellable = viewModel.datePublisher
            .map { date in
                self.dateFormatter.string(from: date)
            }
            .assign(to: \.dateLabel!.stringValue, on: self)
        messageCancellable = viewModel.messagePublisher
            .assign(to: \.messageLabel!.stringValue, on: self)
        
        let sceneNode = GameScene()
        sceneNode.size = CGSize(width: 1920, height: 1600)
        sceneNode.anchorPoint = CGPoint(x: 0.5, y: 0.5)
        sceneNode.scaleMode = .fill
        
        sceneNode.addChild(viewModel.tileMapNode)
        
        spriteKitView.presentScene(sceneNode)
        spriteKitView.ignoresSiblingOrder = true
        spriteKitView.showsFPS = true
        spriteKitView.showsNodeCount = true
        
        (NSApplication.shared.delegate as! AppDelegate).cityModel.run(scenarioID: CityModel.ScenarioID.BOSTON)
        
        // TODO: maximize the window, center the view
        spriteKitView.scroll(NSPoint(x: 0, y: 0))
        
        soundCancellable = viewModel.soundPublisher
            .sink {
                self.viewModel.tileMapNode.addChild($0)
                $0.run(SKAction.sequence([
                    SKAction.play(),
                    SKAction.wait(forDuration: 2.0),
                    SKAction.removeFromParent()
                ]))
            }
    }
}

