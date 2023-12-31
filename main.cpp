#include <iostream>
#include <string>
#include <vector>
#include <angelscript.h>

// Forward declaration
class Game;

// Function to print messages from AngelScript
void MessageCallback(const asSMessageInfo *msg, void *param) {
    const char *type = "INFO";
    if (msg->type == asMSGTYPE_WARNING) type = "WARNING";
    else if (msg->type == asMSGTYPE_ERROR) type = "ERROR";

    std::cout << msg->section << "(" << msg->row << "," << msg->col << ") : " << type << " : " << msg->message << std::endl;
}

// Class representing a room in the game
class Room {
public:
    std::string description;
    Room(std::string desc) : description(desc) {}
};

// Main game class
class Game {
public:
    std::vector<Room> rooms;
    int currentRoom = 0;

    Game() {
        rooms.emplace_back("You are in a dark forest.");
        rooms.emplace_back("You are at a crossroad.");
    }

    void moveToRoom(int roomIndex) {
        if (roomIndex >= 0 && roomIndex < rooms.size()) {
            currentRoom = roomIndex;
            std::cout << rooms[currentRoom].description << std::endl;
        } else {
            std::cout << "You can't go there." << std::endl;
        }
    }
};

// Function to register the Game class and its methods with AngelScript
void RegisterGameAPI(asIScriptEngine *engine) {
    engine->RegisterObjectType("Game", 0, asOBJ_REF | asOBJ_NOCOUNT);
    engine->RegisterObjectMethod("Game", "void moveToRoom(int)", asMETHOD(Game, moveToRoom), asCALL_THISCALL);
}

// Function to execute the AngelScript
void ExecuteScript(asIScriptEngine *engine, const std::string &script, Game &game) {
    asIScriptModule *mod = engine->GetModule("GameModule", asGM_ALWAYS_CREATE);
    mod->AddScriptSection("script", script.c_str());
    mod->Build();

    asIScriptFunction *func = mod->GetFunctionByDecl("void main(Game @)");
    if (func) {
        asIScriptContext *ctx = engine->CreateContext();
        ctx->Prepare(func);
        ctx->SetArgObject(0, &game);
        ctx->Execute();
        ctx->Release();
    }
}

int main() {
    asIScriptEngine *engine = asCreateScriptEngine();
    engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

    RegisterGameAPI(engine);

    Game game;

    // AngelScript code as a C++ string
    std::string script = R"(
        void main(Game @game) {
            game.moveToRoom(0); // Move to the second room
        }
    )";

    ExecuteScript(engine, script, game);

    engine->ShutDownAndRelease();
    return 0;
}
