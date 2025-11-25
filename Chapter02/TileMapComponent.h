#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>

class TileMapComponent : public SpriteComponent
{
    struct TileData
    {
        std::vector<std::vector<int>> tileNum;   // CSVから読み込んだタイル番号
    };
public:
    TileMapComponent(class Actor* owner, int drawOrder = 50);//BGより前に表示するため
    // Update/draw overridden from parent
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;

    // CSV読み込み（タイル番号を2次元配列へ）
    bool LoadCSV(const std::string& filePath);

    // タイルセット画像を設定
    void SetTileSet(SDL_Texture* texture, int tileWidth, int tileHeight);


private:
    std::vector<TileData> mTileDatas;
    SDL_Texture* mTileSet;                  // タイルセット画像
    int mTileWidth;
    int mTileHeight;

    static constexpr int TILESET_COLUMNS = 8; // 横8枚固定
};