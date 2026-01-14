#include <gtest/gtest.h>
#include "todolist/command_parser.h"

using namespace todolist;

class CommandParserTest : public ::testing::Test {
protected:
    CommandParser parser;
};

// Test basic command parsing
TEST_F(CommandParserTest, ParseAddCommand) {
    std::vector<std::string> args = {"add", "Test todo", "Test description"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::ADD);
    ASSERT_EQ(result.args.size(), 2);
    EXPECT_EQ(result.args[0], "Test todo");
    EXPECT_EQ(result.args[1], "Test description");
}

TEST_F(CommandParserTest, ParseListCommand) {
    std::vector<std::string> args = {"list"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::LIST);
    EXPECT_EQ(result.args.size(), 0);
}

TEST_F(CommandParserTest, ParseListWithFilter) {
    std::vector<std::string> args = {"list", "completed"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::LIST);
    ASSERT_EQ(result.args.size(), 1);
    EXPECT_EQ(result.args[0], "completed");
}

TEST_F(CommandParserTest, ParseCompleteCommand) {
    std::vector<std::string> args = {"complete", "42"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::COMPLETE);
    ASSERT_EQ(result.args.size(), 1);
    EXPECT_EQ(result.args[0], "42");
}

TEST_F(CommandParserTest, ParseDeleteCommand) {
    std::vector<std::string> args = {"delete", "1"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::DELETE);
    ASSERT_EQ(result.args.size(), 1);
    EXPECT_EQ(result.args[0], "1");
}

TEST_F(CommandParserTest, ParseSearchCommand) {
    std::vector<std::string> args = {"search", "groceries"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::SEARCH);
    ASSERT_EQ(result.args.size(), 1);
    EXPECT_EQ(result.args[0], "groceries");
}

TEST_F(CommandParserTest, ParseHelpCommand) {
    std::vector<std::string> args = {"help"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::HELP);
    EXPECT_EQ(result.args.size(), 0);
}

TEST_F(CommandParserTest, ParseVersionCommand) {
    std::vector<std::string> args = {"version"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::VERSION);
    EXPECT_EQ(result.args.size(), 0);
}

// Test command aliases
TEST_F(CommandParserTest, AddCommandAliases) {
    auto result1 = parser.parse({"a", "Test"});
    EXPECT_EQ(result1.command, Command::ADD);

    auto result2 = parser.parse({"new", "Test"});
    EXPECT_EQ(result2.command, Command::ADD);
}

TEST_F(CommandParserTest, ListCommandAliases) {
    auto result1 = parser.parse({"l"});
    EXPECT_EQ(result1.command, Command::LIST);

    auto result2 = parser.parse({"ls"});
    EXPECT_EQ(result2.command, Command::LIST);
}

TEST_F(CommandParserTest, CompleteCommandAliases) {
    auto result1 = parser.parse({"c", "1"});
    EXPECT_EQ(result1.command, Command::COMPLETE);

    auto result2 = parser.parse({"done", "1"});
    EXPECT_EQ(result2.command, Command::COMPLETE);
}

TEST_F(CommandParserTest, DeleteCommandAliases) {
    auto result1 = parser.parse({"d", "1"});
    EXPECT_EQ(result1.command, Command::DELETE);

    auto result2 = parser.parse({"del", "1"});
    EXPECT_EQ(result2.command, Command::DELETE);

    auto result3 = parser.parse({"rm", "1"});
    EXPECT_EQ(result3.command, Command::DELETE);
}

TEST_F(CommandParserTest, SearchCommandAliases) {
    auto result1 = parser.parse({"s", "query"});
    EXPECT_EQ(result1.command, Command::SEARCH);

    auto result2 = parser.parse({"find", "query"});
    EXPECT_EQ(result2.command, Command::SEARCH);
}

// Test flags and options
TEST_F(CommandParserTest, ParseFlagsWithValues) {
    std::vector<std::string> args = {"list", "--filter", "completed"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::LIST);
    EXPECT_TRUE(result.hasFlag("filter"));
    EXPECT_EQ(result.getOption("filter"), "completed");
}

TEST_F(CommandParserTest, ParseBooleanFlags) {
    std::vector<std::string> args = {"list", "--all"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::LIST);
    EXPECT_TRUE(result.hasFlag("all"));
    EXPECT_EQ(result.getOption("all"), "true");
}

TEST_F(CommandParserTest, ParseMultipleFlags) {
    std::vector<std::string> args = {"list", "-v", "--filter", "pending"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::LIST);
    EXPECT_TRUE(result.hasFlag("v"));
    EXPECT_TRUE(result.hasFlag("filter"));
    EXPECT_EQ(result.getOption("filter"), "pending");
}

TEST_F(CommandParserTest, ParseShortFlags) {
    std::vector<std::string> args = {"list", "-a"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::LIST);
    EXPECT_TRUE(result.hasFlag("a"));
}

// Test special cases
TEST_F(CommandParserTest, EmptyArgsReturnsHelp) {
    std::vector<std::string> args = {};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::HELP);
}

TEST_F(CommandParserTest, HelpFlagOverridesCommand) {
    auto result1 = parser.parse({"--help"});
    EXPECT_EQ(result1.command, Command::HELP);

    auto result2 = parser.parse({"-h"});
    EXPECT_EQ(result2.command, Command::HELP);
}

TEST_F(CommandParserTest, VersionFlagOverridesCommand) {
    auto result1 = parser.parse({"--version"});
    EXPECT_EQ(result1.command, Command::VERSION);

    auto result2 = parser.parse({"-v"});
    EXPECT_EQ(result2.command, Command::VERSION);
}

TEST_F(CommandParserTest, UnknownCommand) {
    std::vector<std::string> args = {"unknown", "arg1"};
    auto result = parser.parse(args);

    EXPECT_EQ(result.command, Command::UNKNOWN);
}

TEST_F(CommandParserTest, CaseInsensitiveCommands) {
    auto result1 = parser.parse({"ADD", "Test"});
    EXPECT_EQ(result1.command, Command::ADD);

    auto result2 = parser.parse({"LiSt"});
    EXPECT_EQ(result2.command, Command::LIST);
}

// Test utility functions
TEST_F(CommandParserTest, CommandToString) {
    EXPECT_EQ(CommandParser::commandToString(Command::ADD), "add");
    EXPECT_EQ(CommandParser::commandToString(Command::LIST), "list");
    EXPECT_EQ(CommandParser::commandToString(Command::COMPLETE), "complete");
    EXPECT_EQ(CommandParser::commandToString(Command::DELETE), "delete");
    EXPECT_EQ(CommandParser::commandToString(Command::SEARCH), "search");
    EXPECT_EQ(CommandParser::commandToString(Command::HELP), "help");
    EXPECT_EQ(CommandParser::commandToString(Command::VERSION), "version");
    EXPECT_EQ(CommandParser::commandToString(Command::UNKNOWN), "unknown");
}

TEST_F(CommandParserTest, StringToCommand) {
    EXPECT_EQ(CommandParser::stringToCommand("add"), Command::ADD);
    EXPECT_EQ(CommandParser::stringToCommand("list"), Command::LIST);
    EXPECT_EQ(CommandParser::stringToCommand("complete"), Command::COMPLETE);
    EXPECT_EQ(CommandParser::stringToCommand("delete"), Command::DELETE);
    EXPECT_EQ(CommandParser::stringToCommand("search"), Command::SEARCH);
    EXPECT_EQ(CommandParser::stringToCommand("help"), Command::HELP);
    EXPECT_EQ(CommandParser::stringToCommand("version"), Command::VERSION);
    EXPECT_EQ(CommandParser::stringToCommand("invalid"), Command::UNKNOWN);
}

TEST_F(CommandParserTest, GetCommandHelp) {
    std::string help = CommandParser::getCommandHelp(Command::ADD);
    EXPECT_FALSE(help.empty());
    EXPECT_NE(help.find("add"), std::string::npos);
}

TEST_F(CommandParserTest, GetUsage) {
    std::string usage = CommandParser::getUsage();
    EXPECT_FALSE(usage.empty());
    EXPECT_NE(usage.find("Usage"), std::string::npos);
    EXPECT_NE(usage.find("add"), std::string::npos);
    EXPECT_NE(usage.find("list"), std::string::npos);
}
