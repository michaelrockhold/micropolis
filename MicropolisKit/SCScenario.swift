//
//  SCScenario.swift
//  MicropolisSpriteView
//
//  Created by Michael Rockhold on 7/3/23.
//

import Foundation
import CoreGraphics


extension CRISIS_TYPE: Codable {}
extension CRISIS_TIME_MODE: Codable {}
extension WINCRITERION: Codable {}

public struct CodableImageWrapper: Codable {
    
    public enum WrappingError: Error {
        case DECODING_ERROR
        case ENCODING_ERROR
    }

    public let image: CGImage

    public init(image: CGImage) {
        self.image = image
    }

    public init(from decoder: Decoder) throws {
        guard let data = try? (try? decoder.singleValueContainer())?.decode(Data.self) else {
            throw WrappingError.DECODING_ERROR
        }
        guard let image = CGImage.init(pngDataProviderSource: CGDataProvider(data: data as CFData)!,
                                       decode: nil,
                                       shouldInterpolate: true,
                                       intent: .defaultIntent) else {
            throw WrappingError.DECODING_ERROR
        }
        self.image = image
    }

    public func encode(to encoder: Encoder) throws {
        guard let imageData = image.dataProvider?.data else {
            throw WrappingError.ENCODING_ERROR
        }
        var container = encoder.singleValueContainer()
        try container.encode(imageData as Data)
    }
}

enum ScenarioLoadingError : Error {
    case NO_SUCH_RESOURCE
    case CANNOT_READ
    case CANNOT_DECODE
}


public struct SCScenario: Codable {
    let name: String
    let title: String
    let description: String
    let cityTime: Int
    let funds: Int
    let crisisType: CRISIS_TYPE
    let crisisTimeMode: CRISIS_TIME_MODE
    let crisisTime: Int
    let disasterWait: Int
    let scoreWait: Int
    let winCriterion: WINCRITERION
    let winCriterionArg: Int
    let hasFoghorns: Bool
    let cityData: Data
    let icon: CodableImageWrapper
    
    public static func loadScenarioResources() -> [SCScenario] {
        let plistDecoder = PropertyListDecoder()
        var loadedScenarios = [SCScenario]()
        
        var bundles = [Bundle]()
        bundles.append(Bundle.main)
        bundles.append(contentsOf: Bundle.allBundles)
        bundles.append(contentsOf: Bundle.allFrameworks)
        for b in bundles {
            do {
                guard let plistURLs = b.urls(forResourcesWithExtension: "scenario", subdirectory: nil) else {
                    throw ScenarioLoadingError.NO_SUCH_RESOURCE
                }
                for url in plistURLs {
                    guard let data = try? Data.init(contentsOf: url) else {
                        print("error loading \(url)")
                        continue
                    }
                    
                    let value = try plistDecoder.decode(SCScenario.self, from: data)
                    loadedScenarios.append(value)
                }
            } catch {
                print("Unexpected error: \(error).")
            }
        }
        
        return loadedScenarios
    }
}
