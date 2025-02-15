#pragma once

#include "Parser.tab.h"

#include "nixd/Parser/Require.h"

#include <nix/nixexpr.hh>
#include <nix/symbol-table.hh>

#include <filesystem>
#include <utility>

namespace nixd {

std::unique_ptr<ParseData> parse(char *Text, size_t Length,
                                 nix::Pos::Origin Origin,
                                 const nix::SourcePath &BasePath,
                                 ParseState State);

inline std::unique_ptr<ParseData> parse(char *Text, size_t Length,
                                        nix::Pos::Origin Origin,
                                        const nix::SourcePath &BasePath) {
  nix::SymbolTable Symbols;
  nix::PosTable Positions;
  ParseState State{Symbols, Positions};
  return parse(Text, Length, std::move(Origin), BasePath, State);
}

inline std::unique_ptr<ParseData> parse(std::string Text,
                                        nix::Pos::Origin Origin,
                                        const nix::SourcePath &BasePath) {
  Text.append("\0\0", 2);
  return parse(Text.data(), Text.length(), std::move(Origin), BasePath);
}

inline std::unique_ptr<ParseData> parse(std::string Text,
                                        const std::string &Path) {
  Text.append("\0\0", 2);
  auto FSPath = std::filesystem::path(Path);
  auto Origin = nix::CanonPath(FSPath.string());
  auto BasePath = nix::CanonPath(FSPath.remove_filename().string());
  return parse(Text.data(), Text.length(), std::move(Origin),
               std::move(BasePath));
}

inline std::unique_ptr<ParseData> parse(char *Text, size_t Length,
                                        nix::Pos::Origin Origin,
                                        const nix::SourcePath &BasePath,
                                        nix::EvalState &State) {
  auto Data = parse(Text, Length, std::move(Origin), BasePath,
                    ParseState{State.symbols, State.positions});
  return Data;
}

inline std::unique_ptr<ParseData> parse(std::string Text,
                                        nix::Pos::Origin Origin,
                                        const nix::SourcePath &BasePath,
                                        nix::EvalState &State) {
  Text.append("\0\0", 2);
  return parse(Text.data(), Text.length(), std::move(Origin), BasePath, State);
}

} // namespace nixd
