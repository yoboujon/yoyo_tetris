#ifndef HEADER_TETRIS_UI
#define HEADER_TETRIS_UI

#include "lib.h"

#include "event/component.h"

#include "graphics/button.h"
#include "graphics/text_input.h"
#include "graphics/textures.h"


#define VERSION_MAJOR _VERSION_MAJOR
#define VERSION_MINOR _VERSION_MINOR
#define VERSION_PATCH _VERSION_PATCH

constexpr Rectangle TILESET = { 0, 0, 16.0f, 16.0f };
constexpr Rectangle TITLE = { 0, 0, 48.0f, 16.0f };
constexpr Vector2 NEXT_POSITION = {488, 118};
constexpr int CASE_NUM = 5;
constexpr float TILE_RATIO = TETRIS_STAGE.width/((CASE_NUM*7)+(CASE_NUM-1));
constexpr float TITLE_SIZE = 125.0f;
inline Vector2 OFFSET_MENU(float id) { return { 0, 60.0f * id }; };
constexpr Rectangle TILE_DESTINATION = { -(TILE_RATIO * 5.0f), -(TILE_RATIO * 5.0f), SCREEN_WIDTH + (TILE_RATIO * 5.0f), SCREEN_HEIGHT + (TILE_RATIO * 5.0f) };

// TODO : Rename this entire file stages with tetrisStages
// TODO : It will be responsible of the stage elements.
// ! The render pipeline should be done by an entirely different class.
class tetrisUI : public BaseComponent
{
public:
    tetrisUI();
    ~tetrisUI();
    void Display(RendererLayer layer);

    // Setters
    void setScore(uint64_t score);
    void setMultiplicator(uint8_t multiplicator);
    void RenderTile();

    // Getters
    bool quitGame();

private:
    // UI Functions
    textureId textureFromStage(gameStage stage);
    Color colorFromStage(gameStage stage);
    // Scenes
    void TileSet(textureId texture, Color backColor={ 255, 255, 255, 128 });
    void TitleScreen();
    void Game();
    void GameOver();
    void MenuScreen();
    void Settings();

    // Labels/Consts
    const std::string _versionNumber;
    Vector2 _menuCenter;
    uint64_t _score;
    uint8_t _multiplicator;

    // Compositions
    Rectangle _Rect_tetrisStage;
    // Pause/GameOver
    tetrisButton _Btn_restart;
    tetrisButton _Btn_titleScreen;
    tetrisButton _Btn_resume;
    // TitleScreen
    tetrisButton _Btn_Start;
    tetrisButton _Btn_Settings;
    tetrisButton _Btn_Exit;
    // Settings
    tetrisButton _Btn_Back;
    TetrisInputText _TxtInp_FrameRate;

    // Textures
    textureLoader& _textureLoader;

    // UI State
    bool _exit;
    bool _showTiles;
};

#endif // HEADER_TETRIS_UI