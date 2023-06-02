//
//  ContentView.swift
//  MicropolisSpriteView
//
//  Created by Michael Rockhold on 3/16/23.
//

import SwiftUI
import CoreData
import SpriteKit

class GameScene: SKScene {
}

struct ContentView: View {
    @Environment(\.managedObjectContext) private var viewContext

    @FetchRequest(
        sortDescriptors: [NSSortDescriptor(keyPath: \Item.timestamp, ascending: true)],
        animation: .default)
    private var items: FetchedResults<Item>

    var scene: SKScene {
        let scene = GameScene()
        scene.size = CGSize(width: 1200, height: 100)
        scene.scaleMode = .fill
        return scene
    }

    var body: some View {
        SpriteView(scene: scene)
            .frame(width: 1200, height: 1000)
            .ignoresSafeArea()
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
        ContentView().environment(\.managedObjectContext, PersistenceController.preview.container.viewContext)
    }
}
