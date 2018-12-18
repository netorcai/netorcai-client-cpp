# Changelog
All notable changes to this project will be documented in this file.  
The format is based on [Keep a Changelog][changelog].

netorcai-client-cpp adheres to [Semantic Versioning][semver].  
Its public API includes:
- the API of the public functions of the netorcai-client C++ library.
- modifications due to netorcai's metaprotocol.

[//]: =========================================================================
## [Unreleased]
### Added
- New `Client::recvStringNonBlocking` method,
  which allows reading on the socket with a timeout.

[//]: =========================================================================
## 1.0.0 - 2018-11-12
- Initial release.

[//]: =========================================================================
[changelog]: http://keepachangelog.com/en/1.0.0/
[semver]: http://semver.org/spec/v2.0.0.html

[Unreleased]: https://github.com/netorcai/netorcai-client-cpp/compare/v1.0.0...master
