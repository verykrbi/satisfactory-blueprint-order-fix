# Changelog

All notable changes to this mod are listed here. The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and versions follow [Semantic Versioning](https://semver.org/).

## [0.1.1] - 2026-11-19

### Fixed

- The plugin's `Version` field now matches the major number of `SemVersion`, which ficsit.app requires. Version 0.1.0 was rejected there.

## [0.1.0] - 2026-11-19

First release.

### Added

- The blueprint order stored in your save is applied again when the world loads, so blueprints no longer fall back to the order of the files on disk.
- Works per category and sub-category.
- Blueprints that are not in the saved order are placed after the ordered ones.

### Known limitations

- Local play only. Multiplayer and dedicated servers are not handled yet.

[0.1.1]: https://github.com/verykrbi/satisfactory-blueprint-order-fix/compare/v0.1.0...v0.1.1
[0.1.0]: https://github.com/verykrbi/satisfactory-blueprint-order-fix/releases/tag/v0.1.0
