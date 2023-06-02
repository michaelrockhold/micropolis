//
//  MicropolisSpriteViewApp.swift
//  MicropolisSpriteView
//
//  Created by Michael Rockhold on 3/16/23.
//

import SwiftUI

@main
struct MicropolisSpriteViewApp: App {
    let persistenceController = PersistenceController.shared

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environment(\.managedObjectContext, persistenceController.container.viewContext)
        }
    }
}
