#include "TileMapComponent.h"
#include <fstream>
#include <sstream>
#include <iostream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
    : SpriteComponent(owner, drawOrder)
    , mTileSet(nullptr)
    , mTileWidth(0)
    , mTileHeight(0)
{
    LoadCSV("Assets/MapLayer3.csv"); //描画する順番にロードする。
    LoadCSV("Assets/MapLayer2.csv");
    LoadCSV("Assets/MapLayer1.csv");
}

bool TileMapComponent::LoadCSV(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "CSV load error: " << filePath << std::endl;
        return false;
    }

    mTileDatas.resize(mTileDatas.size()+1);

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;
        std::vector<int> row;
        while (std::getline(ss, cell, ','))
        {
            if (!cell.empty())
                row.push_back(std::stoi(cell));
            printf("%d,", std::stoi(cell));
        
        }
        printf("\n");
        mTileDatas[mTileDatas.size()-1].tileNum.push_back(row);
    }

    return true;
}

void TileMapComponent::SetTileSet(SDL_Texture* texture, int tileWidth, int tileHeight)
{
    mTileSet = texture;
    mTileWidth = tileWidth;
    mTileHeight = tileHeight;
}

void TileMapComponent::Update(float deltaTime)
{
    SpriteComponent::Update(deltaTime);
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
    if (!mTileSet || mTileDatas.empty())
        return;
    // レイヤー順に描画
    for (const auto& layer : mTileDatas)
    {
        for (size_t y = 0; y < layer.tileNum.size(); ++y)
        {
            for (size_t x = 0; x < layer.tileNum[y].size(); ++x)
            {
                int id = layer.tileNum[y][x];
                if (id < 0) continue;

                SDL_Rect src;//タイルマップ時に重要になる。
                src.x = (id % TILESET_COLUMNS) * mTileWidth;
                src.y = (id / TILESET_COLUMNS) * mTileHeight;
                src.w = mTileWidth;
                src.h = mTileHeight;

                SDL_Rect dst;
                dst.x = static_cast<int>(x * mTileWidth);
                dst.y = static_cast<int>(y * mTileHeight);
                dst.w = mTileWidth;
                dst.h = mTileHeight;
                //printf("src x:%d y:%d w:%d h:%d\n", src.x, src.y, src.w, src.h);
                //printf("dst x:%d y:%d w:%d h:%d\n", dst.x, dst.y, dst.w, dst.h);

                //指定ではSDL_RenderCopyExを使うとあったSDL_RenderCopyで足りると考えこれを使用した。
                SDL_RenderCopy(renderer, mTileSet, &src, &dst);
            }
        }
    }
}
