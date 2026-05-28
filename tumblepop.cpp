#include <iostream>
#include <fstream>
#include<cstdlib>
#include<ctime>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1136;
int screen_y = 896;

void display_level(RenderWindow& window, char**lvl, Texture& bgTex, Sprite& bgSprite,
                   Texture& blockTexture, Sprite& blockSprite, Texture& slopeRightTexture,
                   Sprite& slopeRightSprite, Texture& slopeLeftTexture, Sprite& slopeLeftSprite,
                   const int height, const int width, const int cell_size)
{
    window.draw(bgSprite);

    for (int pos = 0; pos < height; pos += 1)
    {
        for (int secondary = 0; secondary < width; secondary += 1)
        {
            if (lvl[pos][secondary] == '#')
            {

                blockSprite.setPosition(secondary * cell_size, pos * cell_size);
                window.draw(blockSprite);
            }
            else if (lvl[pos][secondary] == '*')
            {

                slopeRightSprite.setPosition(secondary*cell_size, pos*cell_size);
                window.draw(slopeRightSprite);
            }
            else if (lvl[pos][secondary] == '^')
            {

                slopeLeftSprite.setPosition(secondary*cell_size, pos*cell_size);
                window.draw(slopeLeftSprite);
            }
        }
    }
}

void player_movement(char** lvl, float& player_x, float& player_y, float& velocityY,
                     bool& onGround, const int cell_size, int PlayerWidth, int PlayerHeight,
                     float speed, float jumpStrength, Sprite& PlayerSprite, Texture& PlayerTextureRight,
                     Texture& PlayerTextureLeft, bool& facingRight, int screen_x, int screen_y,
                     const int height, const int width,int selectedplayernum)
{
    float dx = 0;

    if (Keyboard::isKeyPressed(Keyboard::Left)){
        dx = -speed;
if (facingRight) {
PlayerSprite.setTexture(PlayerTextureLeft);
facingRight = false;
}
        int xTexture = 0;
        xTexture = (int)PlayerSprite.getPosition(). x/25 % 5;
        xTexture = xTexture * 34;
        PlayerSprite.setTextureRect(IntRect(xTexture,0,30,40));
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)){
        dx = speed;
if (!facingRight) {
PlayerSprite.setTexture(PlayerTextureRight);
facingRight = true;
}
        int xTexture = 166;
    xTexture = (int)PlayerSprite.getPosition().x/25 % 5;
    xTexture = xTexture * 34;
    PlayerSprite.setTextureRect(IntRect(xTexture, 0, 31, 40));
    }

  if (Keyboard::isKeyPressed(Keyboard::Space) && onGround)
{
    int head_row = (player_y) / cell_size;
    int row2     = head_row - 1;

    int left_col  = (player_x + 5) / cell_size;
    int right_col = (player_x + PlayerWidth - 5) / cell_size;

    if (head_row >= 0)
    {
        bool block1 = false;
        bool block2 = false;

        if (head_row >= 0 && head_row < height)
            block1 = (lvl[head_row][left_col] == '#' || lvl[head_row][right_col] == '#');

        if (row2 >= 0 && row2 < height)
            block2 = (lvl[row2][left_col] == '#' || lvl[row2][right_col] == '#');

        if (block1 && block2)
        {
            velocityY =0;
            onGround = true;

        }
        else
        {

            velocityY = jumpStrength;
            onGround = false;
        }
    }
}

   if (Keyboard::isKeyPressed(Keyboard::Down) && onGround)
{
    int feetrow=(player_y+PlayerHeight)/cell_size;
    int row2 = feetrow + 1;

    int left_col  = (player_x + 5) / cell_size;
    int right_col = (player_x + PlayerWidth - 5) / cell_size;

    if (row2 < height)
    {
        bool block1 = (lvl[feetrow][left_col] == '#' || lvl[feetrow][right_col] == '#');
        bool block2 = (lvl[row2][left_col] == '#' || lvl[row2][right_col] == '#');

        if (block1 && block2)
        {
            velocityY = 0;
            onGround = true;
        }
        else
        {

            velocityY = 67;
            onGround = false;
        }
    }
}

    if (dx != 0)
    {
        float new_x = player_x + dx;

        if (new_x < 0)
            new_x = 0;
        if (new_x + PlayerWidth > screen_x)
            new_x = screen_x - PlayerWidth;

        int feetrow = (int)(player_y + PlayerHeight - 5) / cell_size;

        bool collision = false;

        if (dx < 0)
        {
            int left_col = (int)(new_x) / cell_size;

            if (left_col >= 0 && left_col < width && feetrow >= 0 && feetrow < height)
            {

                if (lvl[feetrow][left_col] == '#')
                {
                    collision = true;
                }
            }
        }
        else if (dx > 0)
        {
            int right_col = (int)(new_x + PlayerWidth) / cell_size;

            if (right_col >= 0 && right_col < width && feetrow >= 0 && feetrow < height)
            {

                if (lvl[feetrow][right_col] == '#')
                {
                    collision = true;
                }
            }
        }

        if (!collision)
        {
            player_x = new_x;

            if (onGround)
            {
                int bottom_row = (int)(player_y + PlayerHeight) / cell_size;
                int mid_col = (int)(player_x + PlayerWidth / 2) / cell_size;

                if (bottom_row >= 0 && bottom_row < height &&
                    mid_col >= 0 && mid_col < width)
                {
                    char tile_below = lvl[bottom_row][mid_col];

                    if (tile_below == '*')
                    {
                        float player_center_x = player_x + PlayerWidth / 2.0f;
                        float tile_x = mid_col * cell_size;
                        float offset_in_tile = player_center_x - tile_x;

                        if (offset_in_tile < 0) offset_in_tile = 0;
                        if (offset_in_tile > cell_size) offset_in_tile = cell_size;

                        float slope_ratio = offset_in_tile / cell_size;
                        float slope_y = (bottom_row * cell_size) + (slope_ratio * cell_size);
                        player_y = slope_y - PlayerHeight;
                    }

                    else if (tile_below == '^')
                    {
                        float player_center_x = player_x + PlayerWidth / 2.0f;
                        float tile_x = mid_col * cell_size;
                        float offset_in_tile = player_center_x - tile_x;

                        if (offset_in_tile < 0) offset_in_tile = 0;
                        if (offset_in_tile > cell_size) offset_in_tile = cell_size;

                        float slope_ratio = 1.0f - (offset_in_tile / cell_size);
                        float slope_y = (bottom_row * cell_size) + (slope_ratio * cell_size);
                        player_y = slope_y - PlayerHeight;
                    }
                    else if (tile_below == '#')
                    {
                        int block_top_y = (bottom_row * cell_size) - PlayerHeight;
                        player_y = block_top_y;
                    }
                }
            }
        }
    }
}

void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity,
                    float& terminal_Velocity, float& player_x, float& player_y, const int cell_size,
                    int& PlayerHeight, int& PlayerWidth, int screen_y, const int height, const int width)
{
    offset_y = player_y;
    offset_y += velocityY;

    if (offset_y < 0) offset_y = 0;
    if (offset_y + PlayerHeight > screen_y)
    {
        offset_y = screen_y - PlayerHeight;
        onGround = true;
        velocityY = 0;
        player_y = offset_y;
        return;
    }

    if (velocityY >= 0)
    {
        int bottom_row = (int)(offset_y + PlayerHeight) / cell_size;
        int left_col = (int)(player_x + 5) / cell_size;
        int right_col = (int)(player_x + PlayerWidth - 5) / cell_size;
        int mid_col = (int)(player_x + PlayerWidth / 2) / cell_size;

        if (bottom_row >= 0 && bottom_row < height &&
            left_col >= 0 && left_col < width &&
            right_col >= 0 && right_col < width &&
            mid_col >= 0 && mid_col < width)
        {
            char bottom_left = lvl[bottom_row][left_col];
            char bottom_right = lvl[bottom_row][right_col];
            char bottom_mid = lvl[bottom_row][mid_col];

            bool landed = false;

            if (bottom_left == '#' || bottom_mid == '#' || bottom_right == '#')
            {
                const float SURFACE_OFFSET = 2.0f;
                int block_top_y = (bottom_row * cell_size) - PlayerHeight;
                player_y = block_top_y;
                onGround = true;
                velocityY = 0;
                landed = true;
            }

            else if (bottom_mid == '*')
            {
                float player_center_x = player_x + PlayerWidth / 2.0f;
                float tile_x = mid_col * cell_size;
                float offset_in_tile = player_center_x - tile_x;

                if (offset_in_tile < 0) offset_in_tile = 0;
                if (offset_in_tile > cell_size) offset_in_tile = cell_size;

                float slope_ratio = offset_in_tile / cell_size;
                float slope_y = (bottom_row * cell_size) + (slope_ratio * cell_size);

                if (offset_y + PlayerHeight >= slope_y - 15)
                {
                    player_y = slope_y - PlayerHeight;
                    onGround = true;
                    velocityY = 0;
                    landed = true;
                }
            }

            else if (bottom_mid == '^')
            {
                float player_center_x = player_x + PlayerWidth / 2.0f;
                float tile_x = mid_col * cell_size;
                float offset_in_tile = player_center_x - tile_x;

                if (offset_in_tile < 0) offset_in_tile = 0;
                if (offset_in_tile > cell_size) offset_in_tile = cell_size;

                float slope_ratio = 1.0f - (offset_in_tile / cell_size);
                float slope_y = (bottom_row * cell_size) + (slope_ratio * cell_size);

                if (offset_y + PlayerHeight >= slope_y - 15)
                {
                    player_y = slope_y - PlayerHeight;
                    onGround = true;
                    velocityY = 0;
                    landed = true;
                }
            }

            if (!landed)
            {
                player_y = offset_y;
                onGround = false;
            }
        }
        else
        {
            player_y = offset_y;
            onGround = false;
        }
    }
    else
    {
        player_y = offset_y;
        onGround = false;
    }

    if (!onGround)
    {
        velocityY += gravity;
        if (velocityY >= terminal_Velocity)
            velocityY = terminal_Velocity;
    }

    if (onGround && velocityY == 0)
    {
        int bottom_row = (int)(player_y + PlayerHeight) / cell_size;
        int left_col = (int)(player_x + 5) / cell_size;
        int right_col = (int)(player_x + PlayerWidth - 5) / cell_size;
        int mid_col = (int)(player_x + PlayerWidth / 2) / cell_size;

        if (bottom_row >= 0 && bottom_row < height &&
            left_col >= 0 && left_col < width &&
            right_col >= 0 && right_col < width &&
            mid_col >= 0 && mid_col < width)
        {
            char left_tile = lvl[bottom_row][left_col];
            char right_tile = lvl[bottom_row][right_col];
            char mid_tile = lvl[bottom_row][mid_col];

            bool on_right_slope = (left_tile == '*' || right_tile == '*' || mid_tile == '*');
            bool on_left_slope = (left_tile == '^' || right_tile == '^' || mid_tile == '^');

            if (on_right_slope)
            {
                float slide_speed = 3.0f;
                player_x += slide_speed;

                int new_left_col = (int)(player_x + 5) / cell_size;
                int new_right_col = (int)(player_x + PlayerWidth - 5) / cell_size;
                int new_mid_col = (int)(player_x + PlayerWidth / 2) / cell_size;

                bool should_stop = false;

                if (new_right_col >= width)
                {

                    should_stop = true;
                }
                else if (new_left_col < width && new_right_col < width && new_mid_col < width)
                {
                    char new_left = lvl[bottom_row][new_left_col];
                    char new_right = lvl[bottom_row][new_right_col];
                    char new_mid = lvl[bottom_row][new_mid_col];

                    int check_row = (int)(player_y + PlayerHeight / 2) / cell_size;
                    if (check_row >= 0 && check_row < height && new_right_col < width)
                    {
                        if (lvl[check_row][new_right_col] == '#')
                        {
                            should_stop = true;
                    }
                    }

                    if (new_left != '*' && new_right != '*' && new_mid != '*')
                    {

                        if ((new_left == '#' && new_right == '#') ||
                            (new_left == '#' && new_mid == '#' && new_right == '-'))
                        {
                            should_stop = true;
                    }
                }
                }

                if (should_stop)
                {
                    player_x -= slide_speed;
                }
            }

            else if (on_left_slope)
            {
                float slide_speed = 3.0f;
                player_x -= slide_speed;

                int new_left_col = (int)(player_x + 5) / cell_size;
                int new_right_col = (int)(player_x + PlayerWidth - 5) / cell_size;
                int new_mid_col = (int)(player_x + PlayerWidth / 2) / cell_size;

                bool should_stop = false;

                if (new_left_col < 0)
                {

                    should_stop = true;
                }
                else if (new_left_col >= 0 && new_right_col >= 0 && new_mid_col >= 0)
                {
                    char new_left = lvl[bottom_row][new_left_col];
                    char new_right = lvl[bottom_row][new_right_col];
                    char new_mid = lvl[bottom_row][new_mid_col];

                    int check_row = (int)(player_y + PlayerHeight / 2) / cell_size;
                    if (check_row >= 0 && check_row < height && new_left_col >= 0)
                    {
                        if (lvl[check_row][new_left_col] == '#')
                        {
                            should_stop = true;
                    }
                    }

                    if (new_left != '^' && new_right != '^' && new_mid != '^')
                    {

                        if ((new_left == '#' && new_right == '#') ||
                            (new_left == '-' && new_mid == '#' && new_right == '#'))
                        {
                            should_stop = true;
                    }
                }
                }

                if (should_stop)
                {
                    player_x += slide_speed;
            }
        }
    }
}
}
bool is_position_safe(char** lvl, int row, int col, int height, int width) {

    if (row <= 0 || row >= height - 1 || col < 0 || col >= width) return false;
    if (lvl[row][col] != '-') return false;
    if (lvl[row + 1][col] != '#') return false;
    if (lvl[row - 1][col] == '#') return false;
    return true;
}

void load_level(char** lvl, int height, int width, int level_number)
{
    // Initialize with '-' (Deep Space)
    for (int pos = 0; pos < height; pos++) {
        for(int secondary = 0; secondary < width; secondary++) {
            lvl[pos][secondary] = '-';
        }
    }

    if(level_number == 1) {
        // ... (Your existing Level 1 code here)
        for(int y = 0; y < 18; y++) lvl[13][y] = '#';
        for(int y = 2; y < 16; y++) {
            lvl[3][y] = '#'; lvl[7][y] = '#'; lvl[11][y] = '#';
        }
        for(int y = 0; y < 4; y++) { lvl[5][y] = '#'; lvl[9][y] = '#'; }
        for(int y = 14; y < 18; y++) { lvl[5][y] = '#'; lvl[9][y] = '#'; }
        for(int y = 7; y < 11; y++) { lvl[5][y] = '#'; lvl[6][y] = '#'; lvl[8][y] = '#'; }
        for(int y = 8; y < 10; y++) { lvl[4][y] = '#'; lvl[9][y] = '#'; lvl[10][y] = '#'; }
    }
    
else if(level_number == 2) {
    // 1. THE CLEAN BOTTOM FLOOR
    // Completely clear of obstacles for easy movement.
    for(int y = 0; y < 18; y++) lvl[13][y] = '#';

    // 2. LOW STEPS (Reachable from the ground - Row 11)
    // 2 blocks high from the ground (Row 13 to Row 11).
    for(int y = 1; y < 5; y++) lvl[11][y] = '#';   // Left Base
    for(int y = 13; y < 17; y++) lvl[11][y] = '#'; // Right Base

    // 3. MID PLATFORMS (Reachable from Low Steps - Row 9)
    // Placed in the center so you can jump from either the left or right base.
    for(int y = 6; y < 12; y++) lvl[9][y] = '#';   

    // 4. UPPER STEPS (Reachable from Mid Platform - Row 7)
    // These overlap slightly with the middle to ensure the jump is easy.
    for(int y = 2; y < 6; y++) lvl[7][y] = '#';    // Left Mid-High
    for(int y = 12; y < 16; y++) lvl[7][y] = '#';  // Right Mid-High

    // 5. HIGH STRUCTURES (Reachable from Upper Steps - Row 5)
    // Centered again to create a pyramid climbing effect.
    for(int y = 7; y < 11; y++) lvl[5][y] = '#';   

    // 6. THE PEAK (The highest point - Row 3)
    // Only 2 blocks high from the Row 5 platform.
    lvl[3][8] = '#'; 
    lvl[3][9] = '#';

    // 7. EXTRA REACHABILITY CONNECTORS (Optional "Safety" blocks)
    // Adding single blocks to act as stairs for tricky gaps
    lvl[10][5] = '#';  // Helper between Row 11 and Row 9
    lvl[8][11] = '#';  // Helper between Row 9 and Row 7
}}
void spawn_ghosts(float ghost_x[], float ghost_y[], float ghost_speed[], bool ghost_movingRight[],
                  Sprite ghost_sprites[], int num_ghosts, char** lvl, int height, int width,
                  int cell_size, Texture& ghostTexture, int PlayerHeight) {
    for (int pos = 0; pos < num_ghosts; pos++){
        bool valid_position = false;

        while (!valid_position) {
            int row = rand() % (height - 2) + 1;
            int col = rand() % (width - 2) + 1;

            // Check for valid ground (#) under air (-)
            if (lvl[row][col] == '-' && lvl[row + 1][col] == '#') {
                float new_x = col * cell_size;
                float new_y = (row + 1) * cell_size - PlayerHeight;

                bool position_taken = false;
                for (int secondary = 0; secondary < pos; secondary++) {
                    if (ghost_x[secondary] == new_x && ghost_y[secondary] == new_y) {
                        position_taken = true;
                        break;
                    }
                }

                if (!position_taken) {
                    ghost_x[pos] = new_x;
                    ghost_y[pos] = new_y;
                    ghost_speed[pos] = 2.5f; // Slightly faster for galactic difficulty
                    ghost_movingRight[pos] = (rand() % 2 == 0);
                    
                    // --- ATTRACTIVE VISUALS START HERE ---
                    ghost_sprites[pos].setTexture(ghostTexture);
                    ghost_sprites[pos].setScale(3.5f, 3.5f); // Slightly larger for better visibility
                    
                    // Give them a "Space Ghost" Cyan tint
                    ghost_sprites[pos].setColor(Color(100, 255, 255, 220)); 
                    
                    valid_position = true;
                }
            }
        }
    }
}
void spawn_chelnovs(float chelnov_x[], float chelnov_y[], float chelnov_speed[],
                    bool chelnov_movingRight[], float chelnov_velocityY[], bool chelnov_onGround[],
                    int chelnov_wait_frames[], bool chelnov_running[], int chelnov_shoot_timer[],
                    bool chelnov_is_shooting[], int chelnov_shoot_phase_timer[],
                    Sprite chelnov_sprites[], int num_chelnovs, char** lvl, int height, int width,
                    int cell_size, Texture& chelnovTextureRight, Texture& chelnovTextureLeft,
                    int PlayerHeight){

    int used_rows[20];
    int num_used_rows = 0;

    for (int pos = 0; pos < num_chelnovs; pos++) {
        bool valid_position = false;
        int attempts = 0;

        while (!valid_position && attempts < 100) {
            attempts++;

            int row = rand() % (height - 2) + 1;
            int col = rand() % (width - 2) + 1;

            bool row_taken = false;
            for (int secondary = 0; secondary < num_used_rows; secondary++) {
                if (used_rows[secondary] == row) {
                    row_taken = true;
                    break;
                }
            }

            if (row_taken) continue;

            if (lvl[row][col] == '-' && lvl[row + 1][col] == '#') {
                float new_x = col * cell_size;
                float new_y = (row + 1) * cell_size - PlayerHeight;

                chelnov_x[pos] = new_x;
                chelnov_y[pos] = new_y;
                chelnov_speed[pos] = 1.5;
                chelnov_movingRight[pos] = (rand() % 2 == 0);
                chelnov_velocityY[pos] = 0;
                chelnov_onGround[pos] = true;
                chelnov_wait_frames[pos] = 0;
                chelnov_running[pos] = false;

                chelnov_shoot_timer[pos] = 240;
                chelnov_is_shooting[pos] = false;
                chelnov_shoot_phase_timer[pos] = 0;

                if (chelnov_movingRight[pos]) {
                    chelnov_sprites[pos].setTexture(chelnovTextureRight);
                } else {
                    chelnov_sprites[pos].setTexture(chelnovTextureLeft);
                }
                chelnov_sprites[pos].setScale(3, 3);

                used_rows[num_used_rows] = row;
                num_used_rows++;
                valid_position = true;
            }
        }
    }
}
void draw_pause_menu(RenderWindow& window, Font& font, bool& is_paused, bool& returnToMenu) {
    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 150));
    window.draw(overlay);
auto stylePauseBtn = [&](RectangleShape& b, Text& t, string str, float y) {
    // 1. Box Dimensions
    float btnWidth = 600.0f;
    float btnHeight = 100.0f;
    
    b.setSize(Vector2f(btnWidth, btnHeight)); 
    
    // 2. PERFECT CENTERING: Origin should be exactly half of Size
    b.setOrigin(btnWidth / 2.0f, btnHeight / 2.0f); 
    
    // 3. POSITIONING: Center of screen horizontally
    b.setPosition(window.getSize().x / 2.0f, y); 
    
    b.setFillColor(Color(30, 30, 60));
    b.setOutlineThickness(3);
    b.setOutlineColor(Color::Cyan);

    // 4. TEXT ALIGNMENT
    t.setFont(font);
    t.setString(str);
    t.setCharacterSize(45); 
    t.setFillColor(Color::White);

    // Center the text inside the button
    FloatRect textBounds = t.getLocalBounds();
    t.setOrigin(textBounds.left + textBounds.width / 2.0f, 
               textBounds.top + textBounds.height / 2.0f);
    t.setPosition(b.getPosition());
};

    RectangleShape contBtn, exitBtn;
    Text contText, exitText;

    stylePauseBtn(contBtn, contText, "CONTINUE", window.getSize().y / 2.0f - 60);
    stylePauseBtn(exitBtn, exitText, "GO TO HUB", window.getSize().y / 2.0f + 60);

    Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

    // Interaction
    if (contBtn.getGlobalBounds().contains(mouse)) contText.setFillColor(Color::White);
    if (exitBtn.getGlobalBounds().contains(mouse)) exitText.setFillColor(Color::White);

    if (Mouse::isButtonPressed(Mouse::Left)) {
        if (contBtn.getGlobalBounds().contains(mouse)) is_paused = false;
        if (exitBtn.getGlobalBounds().contains(mouse)) returnToMenu = true; // Sets flag to go back
    }

    window.draw(contBtn); window.draw(contText);
    window.draw(exitBtn); window.draw(exitText);
}
void spawn_skeletons(float skeleton_x[], float skeleton_y[], float skeleton_speed[],
                     bool skeleton_movingRight[], float skeleton_velocityY[], bool skeleton_onGround[],
                     int skeleton_wait_frames[], bool skeleton_running[], Sprite skeleton_sprites[],
                     int num_skeletons, char** lvl, int height, int width, int cell_size,
                     Texture& skeletonTextureRight, Texture& skeletonTextureLeft, int PlayerHeight) {

    int used_rows[20];
    int num_used_rows = 0;

    for (int pos = 0; pos < num_skeletons; pos++) {
        bool valid_position = false;
        int attempts = 0;
        int max_attempts = 100;

        while (!valid_position && attempts < max_attempts) {
            attempts++;

            int row = rand() % (height - 2) + 1;
            int col = rand() % (width - 2) + 1;

            bool row_taken = false;
            for (int secondary = 0; secondary < num_used_rows; secondary++) {
                if (used_rows[secondary] == row) {
                    row_taken = true;
                    break;
                }
            }

            if (row_taken) continue;

            // Logic to find a valid platform ('#') with air ('-') above it
            if (lvl[row][col] == '-' && lvl[row + 1][col] == '#') {
                float new_x = col * cell_size;
                float new_y = (row + 1) * cell_size - PlayerHeight;

                skeleton_x[pos] = new_x;
                skeleton_y[pos] = new_y;
                skeleton_speed[pos] = 1.8f; // Slightly increased for "Galactic" difficulty
                skeleton_movingRight[pos] = (rand() % 2 == 0);
                skeleton_velocityY[pos] = 0;
                skeleton_onGround[pos] = true;
                skeleton_wait_frames[pos] = 0;
                skeleton_running[pos] = false;

                // --- GALACTIC VISUALS ---
                if (skeleton_movingRight[pos]) {
                    skeleton_sprites[pos].setTexture(skeletonTextureRight);
                } else {
                    skeleton_sprites[pos].setTexture(skeletonTextureLeft);
                }

                // Apply a "Cyber-Energy" tint (Purple/Magenta) to make them look attractive and space-themed
                skeleton_sprites[pos].setColor(Color(255, 100, 255, 230)); 
                skeleton_sprites[pos].setScale(3.2f, 3.2f); // Slightly larger for better arcade visibility
                
                used_rows[num_used_rows] = row;
                num_used_rows++;
                valid_position = true;
            }
        }
        
        // Fallback logic if row-exclusive spawning fails
        if (!valid_position) {
            while (!valid_position) {
                int row = rand() % (height - 2) + 1;
                int col = rand() % (width - 2) + 1;

                if (lvl[row][col] == '-' && lvl[row + 1][col] == '#') {
                    skeleton_x[pos] = col * cell_size;
                    skeleton_y[pos] = (row + 1) * cell_size - PlayerHeight;
                    skeleton_speed[pos] = 1.8f;
                    skeleton_movingRight[pos] = (rand() % 2 == 0);
                    skeleton_velocityY[pos] = 0;
                    skeleton_onGround[pos] = true;
                    skeleton_wait_frames[pos] = 0;
                    skeleton_running[pos] = false;

                    if (skeleton_movingRight[pos]) {
                        skeleton_sprites[pos].setTexture(skeletonTextureRight);
                    } else {
                        skeleton_sprites[pos].setTexture(skeletonTextureLeft);
                    }

                    // Matching the Galactic "Alien Undead" look
                    skeleton_sprites[pos].setColor(Color(255, 100, 255, 230));
                    skeleton_sprites[pos].setScale(3.2f, 3.2f);

                    valid_position = true;
                }
            }
        }
    }
}
void update_chelnovs(float chelnov_x[], float chelnov_y[], float chelnov_speed[],
                     bool chelnov_movingRight[], float chelnov_velocityY[], bool chelnov_onGround[],
                     int chelnov_wait_frames[], bool chelnov_running[], int chelnov_shoot_timer[],
                     bool chelnov_is_shooting[], int chelnov_shoot_phase_timer[],
                     Sprite chelnov_sprites[], int num_chelnovs, char** lvl, int height, int width,
                     int cell_size, int screen_x, int screen_y, Texture& chelnovTextureRight,
                     Texture& chelnovTextureLeft, bool enemy_active[], int chelnov_start_index,
                     bool enemy_stunned[], bool enemy_pulling[], float player_x, float player_y,
                     float projectile_x[], float projectile_y[], float projectile_speed_x[],
                     float projectile_speed_y[], bool projectile_active[], int max_projectiles) {

    const float gravity = 1;
    const float jumpStrength = -12;
    const int chelnov_width = 96;
    const int chelnov_height = 120;
    const int wait_time = 60;
    const float walk_speed = 1.5;

    for (int pos = 0; pos < num_chelnovs; pos++) {
        int enemy_index = chelnov_start_index + pos;

        if (!enemy_active[enemy_index]) continue;
        if (enemy_stunned[enemy_index] || enemy_pulling[enemy_index]) {
            chelnov_sprites[pos].setPosition(chelnov_x[pos], chelnov_y[pos]);
            continue;
        }

        if (!chelnov_is_shooting[pos]) {
            chelnov_shoot_timer[pos]--;

            if (chelnov_shoot_timer[pos] <= 0) {
                chelnov_is_shooting[pos] = true;
                chelnov_shoot_phase_timer[pos] = 60;
                chelnov_wait_frames[pos] = 60;
            }
        } else {

            chelnov_shoot_phase_timer[pos]--;

            if (chelnov_shoot_phase_timer[pos] == 55) {

                for (int p = 0; p < max_projectiles; p++) {
                    if (!projectile_active[p]) {
                        projectile_active[p] = true;

                        projectile_x[p] = chelnov_x[pos] + chelnov_width / 2 - 16;
                        projectile_y[p] = chelnov_y[pos] + chelnov_height / 2 - 16;

                        float player_center_x = player_x + 48;
                        float chelnov_center_x = chelnov_x[pos] + 48;

                        float projectile_speed = 8.0f;

                        if (player_center_x > chelnov_center_x) {

                            projectile_speed_x[p] = projectile_speed;
                        } else {

                            projectile_speed_x[p] = -projectile_speed;
                        }

                        projectile_speed_y[p] = 0.0f;

                        break;
                    }
                }
            }

            if (chelnov_shoot_phase_timer[pos] <= 0) {
                chelnov_is_shooting[pos] = false;
                chelnov_shoot_timer[pos] = 240;
            }
        }

        if (chelnov_wait_frames[pos] > 0) {
            chelnov_wait_frames[pos]--;

            if (!chelnov_onGround[pos]) {
                chelnov_velocityY[pos] += gravity;
                if (chelnov_velocityY[pos] > 15) chelnov_velocityY[pos] = 15;
            }

            chelnov_y[pos] += chelnov_velocityY[pos];

            int current_row = (int)(chelnov_y[pos] + chelnov_height) / cell_size;
            int left_col = (int)(chelnov_x[pos] + 5) / cell_size;
            int right_col = (int)(chelnov_x[pos] + chelnov_width - 5) / cell_size;

            if (current_row >= 0 && current_row < height && left_col >= 0 &&
                left_col < width && right_col < width) {
                if (lvl[current_row][left_col] == '#' || lvl[current_row][right_col] == '#') {
                    int block_top = current_row * cell_size - chelnov_height;
                    chelnov_y[pos] = block_top;
                    chelnov_onGround[pos] = true;
                    chelnov_velocityY[pos] = 0;
                } else {
                    chelnov_onGround[pos] = false;
                }
            }

            chelnov_sprites[pos].setPosition(chelnov_x[pos], chelnov_y[pos]);
            continue;
        }

          float next_x = chelnov_x[pos];
        if (chelnov_movingRight[pos]) {
            next_x += chelnov_speed[pos];

            int frame = (int)(chelnov_x[pos] / 10) % 5;
    int xTexture = frame * 32;
    chelnov_sprites[pos].setTextureRect(IntRect(xTexture, 0, 32, 42));
} else {
    next_x -= chelnov_speed[pos];
    int frame = (int)(chelnov_x[pos] / 10) % 5;
    int xTexture = frame * 32;
    chelnov_sprites[pos].setTextureRect(IntRect(xTexture, 0, 32, 42));
}

        if (next_x < 0 || next_x + chelnov_width > screen_x) {
            chelnov_movingRight[pos] = !chelnov_movingRight[pos];
            chelnov_wait_frames[pos] = wait_time;

            if (chelnov_movingRight[pos]) {
                chelnov_sprites[pos].setTexture(chelnovTextureRight);
            } else {
                chelnov_sprites[pos].setTexture(chelnovTextureLeft);
            }

            chelnov_sprites[pos].setPosition(chelnov_x[pos], chelnov_y[pos]);
            continue;
        }

        int current_col = (int)(next_x + chelnov_width / 2) / cell_size;
        int current_row = (int)(chelnov_y[pos] + chelnov_height / 2) / cell_size;
        int below_row = current_row + 1;

        bool wall_ahead = false;
        bool no_floor = false;

        int check_col_ahead;
        if (chelnov_movingRight[pos]) {
            check_col_ahead = (int)(next_x + chelnov_width) / cell_size;
        } else {
            check_col_ahead = (int)(next_x) / cell_size;
        }

        if (check_col_ahead >= 0 && check_col_ahead < width && current_row >= 0 && current_row < height) {
            if (lvl[current_row][check_col_ahead] == '#') {
                wall_ahead = true;
            }
        }

        int check_col = current_col;
        if (chelnov_movingRight[pos]) {
            check_col = (int)(next_x + chelnov_width) / cell_size;
        } else {
            check_col = (int)(next_x) / cell_size;
        }

        if (below_row >= 0 && below_row < height && check_col >= 0 && check_col < width) {
            if (lvl[below_row][check_col] == '-') {
                no_floor = true;
            }
        } else {
            no_floor = true;
        }

        if (wall_ahead || no_floor) {
            chelnov_movingRight[pos] = !chelnov_movingRight[pos];
            chelnov_wait_frames[pos] = wait_time;

            if (chelnov_movingRight[pos]) {
                chelnov_sprites[pos].setTexture(chelnovTextureRight);
            } else {
                chelnov_sprites[pos].setTexture(chelnovTextureLeft);
            }
        } else {
            chelnov_x[pos] = next_x;
        }

        if (rand() % 150 == 0) {
            chelnov_movingRight[pos] = !chelnov_movingRight[pos];
            chelnov_wait_frames[pos] = wait_time;

            if (chelnov_movingRight[pos]) {
                chelnov_sprites[pos].setTexture(chelnovTextureRight);
                chelnov_sprites[pos].setTextureRect(IntRect(0, 0, 32, 42));
            } else {
                chelnov_sprites[pos].setTexture(chelnovTextureLeft);
                chelnov_sprites[pos].setTextureRect(IntRect(0, 0, 32, 42));
            }
        }

        if (!chelnov_onGround[pos]) {
            chelnov_velocityY[pos] += gravity;
            if (chelnov_velocityY[pos] > 15) chelnov_velocityY[pos] = 15;
        }

        chelnov_y[pos] += chelnov_velocityY[pos];

        int current_row_check = (int)(chelnov_y[pos] + chelnov_height) / cell_size;
        int left_col = (int)(chelnov_x[pos] + 5) / cell_size;
        int right_col = (int)(chelnov_x[pos] + chelnov_width - 5) / cell_size;

        if (current_row_check >= 0 && current_row_check < height && left_col >= 0 &&
            left_col < width && right_col < width) {
            if (lvl[current_row_check][left_col] == '#' || lvl[current_row_check][right_col] == '#') {
                int block_top = current_row_check * cell_size - chelnov_height;
                chelnov_y[pos] = block_top;
                chelnov_onGround[pos] = true;
                chelnov_velocityY[pos] = 0;
            } else {
                chelnov_onGround[pos] = false;
            }
        }

        chelnov_sprites[pos].setPosition(chelnov_x[pos], chelnov_y[pos]);
    }
}

void update_projectiles(float projectile_x[], float projectile_y[], float projectile_speed_x[],
                       float projectile_speed_y[], bool projectile_active[], int max_projectiles,
                       int screen_x, int screen_y, char** lvl, int height, int width, int cell_size,
                       float player_x, float player_y, int PlayerWidth, int PlayerHeight,
                       bool& player_is_dead) {

    for (int pos = 0; pos < max_projectiles; pos++) {
        if (!projectile_active[pos]) continue;

        float old_x = projectile_x[pos];
        float old_y = projectile_y[pos];

        projectile_x[pos] += projectile_speed_x[pos];

        projectile_speed_y[pos] += 0.8f;
        projectile_y[pos] += projectile_speed_y[pos];

        if (projectile_y[pos] > screen_y) {
            projectile_active[pos] = false;
            continue;
        }

        if (projectile_x[pos] < 0) {
            projectile_x[pos] = 0;
            projectile_speed_x[pos] = -projectile_speed_x[pos];

            if (projectile_y[pos] + 32 >= screen_y - 64) {
                projectile_active[pos] = false;
                continue;
            }
        }
        if (projectile_x[pos] + 32 > screen_x) {
            projectile_x[pos] = screen_x - 32;
            projectile_speed_x[pos] = -projectile_speed_x[pos];

            if (projectile_y[pos] + 32 >= screen_y - 64) {
                projectile_active[pos] = false;
                continue;
            }
        }

        int projectile_bottom = (int)(projectile_y[pos] + 32);
        int projectile_bottom_row = projectile_bottom / cell_size;
        int projectile_left_col = (int)(projectile_x[pos] + 4) / cell_size;
        int projectile_right_col = (int)(projectile_x[pos] + 28) / cell_size;
        int projectile_center_col = (int)(projectile_x[pos] + 16) / cell_size;

        bool on_ground = false;

        if (projectile_speed_y[pos] > 0) {
            if (projectile_bottom_row >= 0 && projectile_bottom_row < height &&
                projectile_center_col >= 0 && projectile_center_col < width) {

                if (lvl[projectile_bottom_row][projectile_left_col] == '#' ||
                    lvl[projectile_bottom_row][projectile_center_col] == '#' ||
                    lvl[projectile_bottom_row][projectile_right_col] == '#') {

                    int platform_top_y = (projectile_bottom_row * cell_size) - 32;
                    projectile_y[pos] = platform_top_y;
                    projectile_speed_y[pos] = 0;
                    on_ground = true;

                    if (projectile_bottom_row >= height - 1) {

                        if (projectile_x[pos] <= 64 || projectile_x[pos] + 32 >= screen_x - 64) {
                            projectile_active[pos] = false;
                            continue;
                        }
                    }
                }
            }
        }

        int projectile_center_row = (int)(projectile_y[pos] + 16) / cell_size;
        int left_col = (int)(projectile_x[pos]) / cell_size;
        int right_col = (int)(projectile_x[pos] + 32) / cell_size;

        if (projectile_center_row >= 0 && projectile_center_row < height) {

            if (projectile_speed_x[pos] > 0 && right_col < width &&
                lvl[projectile_center_row][right_col] == '#') {
                projectile_x[pos] = old_x;
                projectile_speed_x[pos] = -projectile_speed_x[pos];

                if (on_ground && right_col >= width - 1) {
                    projectile_active[pos] = false;
                    continue;
                }
            }

            else if (projectile_speed_x[pos] < 0 && left_col >= 0 &&
                     lvl[projectile_center_row][left_col] == '#') {
                projectile_x[pos] = old_x;
                projectile_speed_x[pos] = -projectile_speed_x[pos];

                if (on_ground && left_col <= 0) {
                    projectile_active[pos] = false;
                    continue;
                }
            }
        }

        int top_row = (int)(projectile_y[pos]) / cell_size;
        if (projectile_speed_y[pos] < 0 && top_row >= 0 && top_row < height &&
            projectile_center_col >= 0 && projectile_center_col < width) {
            if (lvl[top_row][projectile_center_col] == '#') {
                projectile_y[pos] = old_y;
                projectile_speed_y[pos] = 2.0f;
            }
        }

        if (projectile_x[pos] < player_x + PlayerWidth &&
            projectile_x[pos] + 32 > player_x &&
            projectile_y[pos] < player_y + PlayerHeight &&
            projectile_y[pos] + 32 > player_y) {

            player_is_dead = true;
            projectile_active[pos] = false;
        }
    }
}

void update_skeletons(float skeleton_x[], float skeleton_y[], float skeleton_speed[],
                      bool skeleton_movingRight[], float skeleton_velocityY[], bool skeleton_onGround[],
                      int skeleton_wait_frames[], bool skeleton_running[], Sprite skeleton_sprites[],
                      int num_skeletons, char** lvl, int height, int width, int cell_size,
                      int screen_x, int screen_y, Texture& skeletonTextureRight, Texture& skeletonTextureLeft,
                      bool enemy_active[], int skeleton_start_index, bool enemy_stunned[], bool enemy_pulling[]) {

    const float gravity = 1.0f;
    const float jumpStrength = -16.0f;
    const int skeleton_width = 96;
    const int skeleton_height = 102;
    const int wait_time = 45; // Reduced wait time for more activity
    const float walk_speed = 1.8f;
    const float run_speed = 3.8f;

    for (int pos = 0; pos < num_skeletons; pos++) {
        int enemy_index = skeleton_start_index + pos;

        if (!enemy_active[enemy_index]) continue;
        if (enemy_stunned[enemy_index] || enemy_pulling[enemy_index]) {
            skeleton_sprites[pos].setPosition(skeleton_x[pos], skeleton_y[pos]);
            continue;
        }

        // --- 1. MOVEMENT & IDLE STATE ---
        if (skeleton_wait_frames[pos] > 0 && skeleton_onGround[pos]) {
            skeleton_wait_frames[pos]--;
        } else {
            // Apply Horizontal Movement
            float next_x = skeleton_x[pos] + (skeleton_movingRight[pos] ? skeleton_speed[pos] : -skeleton_speed[pos]);

            // Screen Boundary Check (Fixes sticking at edges)
            if (next_x < 0 || next_x + skeleton_width > screen_x) {
                skeleton_movingRight[pos] = !skeleton_movingRight[pos];
                skeleton_wait_frames[pos] = wait_time;
            } else {
                // Wall Collision Check
                int check_col = (skeleton_movingRight[pos]) ? (int)(next_x + skeleton_width - 10) / cell_size : (int)(next_x + 10) / cell_size;
                int mid_row = (int)(skeleton_y[pos] + skeleton_height / 2) / cell_size;

                if (mid_row >= 0 && mid_row < height && check_col >= 0 && check_col < width && lvl[mid_row][check_col] == '#') {
                    skeleton_movingRight[pos] = !skeleton_movingRight[pos];
                    skeleton_wait_frames[pos] = wait_time;
                } else {
                    skeleton_x[pos] = next_x; // Move successfully
                }
            }
        }

        // --- 2. JUMP LOGIC (UP) ---
        // Occurs only if on ground, not waiting, and a platform is detected above
        if (skeleton_onGround[pos] && skeleton_wait_frames[pos] <= 0 && rand() % 150 == 0) {
            int head_row = (int)(skeleton_y[pos]) / cell_size;
            int current_col = (int)(skeleton_x[pos] + skeleton_width / 2) / cell_size;

            bool platformAbove = false;
            for (int r = 2; r <= 6; r++) { // Scan 2 to 6 blocks up
                if (head_row - r >= 0 && lvl[head_row - r][current_col] == '#') {
                    platformAbove = true;
                    break;
                }
            }
            if (platformAbove) {
                skeleton_velocityY[pos] = jumpStrength;
                skeleton_onGround[pos] = false;
            }
        }

        // --- 3. DROP LOGIC (DOWN) ---
        // Skeletons will occasionally decide to "fall through" the air to a lower floor
        if (skeleton_onGround[pos] && rand() % 500 == 0) {
            // Simply nudge them down so they aren't "on ground" anymore
            skeleton_y[pos] += 5; 
            skeleton_onGround[pos] = false;
            skeleton_velocityY[pos] = 5.0f; // Start with a little downward push
        }

        // --- 4. VERTICAL PHYSICS & GRAVITY ---
        if (!skeleton_onGround[pos]) {
            skeleton_velocityY[pos] += gravity;
            if (skeleton_velocityY[pos] > 15) skeleton_velocityY[pos] = 15;
            skeleton_y[pos] += skeleton_velocityY[pos];
        }

        // --- 5. FLOOR COLLISION ---
        int feet_row = (int)(skeleton_y[pos] + skeleton_height) / cell_size;
        int left_col = (int)(skeleton_x[pos] + 20) / cell_size;
        int right_col = (int)(skeleton_x[pos] + skeleton_width - 20) / cell_size;

        if (feet_row >= 0 && feet_row < height && left_col >= 0 && right_col < width) {
            // Land only if moving downwards
            if (skeleton_velocityY[pos] >= 0 && (lvl[feet_row][left_col] == '#' || lvl[feet_row][right_col] == '#')) {
                skeleton_y[pos] = feet_row * cell_size - skeleton_height;
                skeleton_onGround[pos] = true;
                skeleton_velocityY[pos] = 0;
            } else {
                skeleton_onGround[pos] = false;
            }
        }

        // --- 6. TEXTURE & SPRITE UPDATE ---
        skeleton_sprites[pos].setTexture(skeleton_movingRight[pos] ? skeletonTextureRight : skeletonTextureLeft);
        skeleton_sprites[pos].setPosition(skeleton_x[pos], skeleton_y[pos]);
    }
}

void spawn_invisible_men(float invisible_x[], float invisible_y[], float invisible_speed[],
                         bool invisible_movingRight[], int invisible_wait_frames[],
                         bool invisible_teleporting[], bool invisible_visible[],
                         int invisible_visibility_timer[], int invisible_anim_frame[],
                         int invisible_anim_timer[], Sprite invisible_sprites[],
                         int num_invisible, char** lvl, int height, int width, int cell_size,
                         Texture& invisibleTextureRight, Texture& invisibleTextureLeft,
                         int PlayerHeight) {

    for (int pos = 0; pos < num_invisible; pos++) {
        bool valid_position = false;

        while (!valid_position) {
            int row = rand() % (height - 2) + 1;
            int col = rand() % (width - 2) + 1;

            if (lvl[row][col] == '-' && lvl[row + 1][col] == '#') {
                float new_x = col * cell_size;
                float new_y = (row + 1) * cell_size - PlayerHeight;

                bool position_taken = false;
                for (int secondary = 0; secondary < pos; secondary++) {
                    if (invisible_x[secondary] == new_x && invisible_y[secondary] == new_y) {
                        position_taken = true;
                        break;
                    }
                }

                if (!position_taken) {
                    invisible_x[pos] = new_x;
                    invisible_y[pos] = new_y;
                    invisible_speed[pos] = 2.0f;
                    invisible_movingRight[pos] = (rand() % 2 == 0);
                    invisible_wait_frames[pos] = 0;
                    invisible_teleporting[pos] = false;
                    invisible_visible[pos] = true;
                    invisible_visibility_timer[pos] = rand() % 180 + 120;
                    invisible_anim_frame[pos] = 0;
                    invisible_anim_timer[pos] = 0;

                    if (invisible_movingRight[pos]) {
                        invisible_sprites[pos].setTexture(invisibleTextureRight);
                    } else {
                        invisible_sprites[pos].setTexture(invisibleTextureLeft);
                    }
                    invisible_sprites[pos].setScale(3, 3);
                    valid_position = true;
                }
            }
        }
    }
}

void update_invisible_men(float invisible_x[], float invisible_y[], float invisible_speed[],
                         bool invisible_movingRight[], int invisible_wait_frames[],
                         bool invisible_teleporting[], bool invisible_visible[],
                         int invisible_visibility_timer[], int invisible_anim_frame[],
                         int invisible_anim_timer[], float invisible_target_y[],
                         Sprite invisible_sprites[], int num_invisible, char** lvl,
                         int height, int width, int cell_size, int screen_x, int screen_y,
                         bool enemy_active[], int invisible_start_index,
                         bool enemy_stunned[], bool enemy_pulling[],
                         Texture& invisibleTextureRight, Texture& invisibleTextureLeft,
                         Texture invisibleStopTextures[], Texture invisibleFadeTextures[]) {

    const int invisible_width = 96;
    const int invisible_height = 102;
    const int wait_time = 60;
    const int frame_duration = 10;

    for (int pos = 0; pos < num_invisible; pos++) {
        int enemy_index = invisible_start_index + pos;

        if (!enemy_active[enemy_index]) continue;

        if (enemy_stunned[enemy_index] || enemy_pulling[enemy_index]) {
            invisible_sprites[pos].setPosition(invisible_x[pos], invisible_y[pos]);
            continue;
        }

        if (!invisible_teleporting[pos]) {
            invisible_visibility_timer[pos]--;
            if (invisible_visibility_timer[pos] <= 0) {
                invisible_visible[pos] = !invisible_visible[pos];

                if (invisible_visible[pos]) {
                    invisible_visibility_timer[pos] = rand() % 180 + 120;
                } else {
                    invisible_visibility_timer[pos] = rand() % 120 + 60;
                }
            }
        }

        if (invisible_teleporting[pos]) {
            invisible_anim_timer[pos]++;

            if (invisible_anim_frame[pos] < 3) {
                if (invisible_anim_timer[pos] >= frame_duration) {
                    invisible_anim_timer[pos] = 0;
                    invisible_anim_frame[pos]++;
                }
                invisible_sprites[pos].setTexture(invisibleStopTextures[invisible_anim_frame[pos]]);
                invisible_visible[pos] = true;
            }

            else if (invisible_anim_frame[pos] >= 3 && invisible_anim_frame[pos] < 7) {
                if (invisible_anim_timer[pos] >= frame_duration) {
                    invisible_anim_timer[pos] = 0;
                    invisible_anim_frame[pos]++;
                }
                int fade_frame = invisible_anim_frame[pos] - 3;
                invisible_sprites[pos].setTexture(invisibleFadeTextures[fade_frame]);
                invisible_visible[pos] = true;
            }

            else if (invisible_anim_frame[pos] == 7) {

                invisible_y[pos] = invisible_target_y[pos];
                invisible_visible[pos] = false;
                invisible_anim_frame[pos]++;
                invisible_anim_timer[pos] = 0;
            }

            else if (invisible_anim_frame[pos] >= 8 && invisible_anim_frame[pos] < 12) {
                if (invisible_anim_timer[pos] >= frame_duration) {
                    invisible_anim_timer[pos] = 0;
                    invisible_anim_frame[pos]++;
                }
                int fade_frame = 11 - invisible_anim_frame[pos];
                invisible_sprites[pos].setTexture(invisibleFadeTextures[fade_frame]);
                invisible_visible[pos] = true;
            }

            else {

                if (invisible_movingRight[pos]) {
                    invisible_sprites[pos].setTexture(invisibleTextureRight);
                } else {
                    invisible_sprites[pos].setTexture(invisibleTextureLeft);
                }
                invisible_teleporting[pos] = false;
                invisible_anim_frame[pos] = 0;
                invisible_anim_timer[pos] = 0;
                invisible_target_y[pos] = invisible_y[pos];
                invisible_visible[pos] = true;
            }

            invisible_sprites[pos].setPosition(invisible_x[pos], invisible_y[pos]);
            continue;
        }

        float next_x = invisible_x[pos];
        if (invisible_movingRight[pos]) {
            next_x += invisible_speed[pos];
        } else {
            next_x -= invisible_speed[pos];
        }

        if (next_x < 0 || next_x + invisible_width > screen_x) {
            invisible_movingRight[pos] = !invisible_movingRight[pos];

            if (invisible_movingRight[pos]) {
                invisible_sprites[pos].setTexture(invisibleTextureRight);
            } else {
                invisible_sprites[pos].setTexture(invisibleTextureLeft);
            }

            invisible_sprites[pos].setPosition(invisible_x[pos], invisible_y[pos]);
            continue;
        }

        int current_col = (int)(next_x + invisible_width / 2) / cell_size;
        int current_row = (int)(invisible_y[pos] + invisible_height / 2) / cell_size;
        int below_row = current_row + 1;

        bool wall_ahead = false;
        bool no_floor = false;

        int check_col_ahead;
        if (invisible_movingRight[pos]) {
            check_col_ahead = (int)(next_x + invisible_width) / cell_size;
        } else {
            check_col_ahead = (int)(next_x) / cell_size;
        }

        if (check_col_ahead >= 0 && check_col_ahead < width && current_row >= 0 && current_row < height) {
            if (lvl[current_row][check_col_ahead] == '#') {
                wall_ahead = true;
            }
        }

        int check_col = current_col;
        if (invisible_movingRight[pos]) {
            check_col = (int)(next_x + invisible_width) / cell_size;
        } else {
            check_col = (int)(next_x) / cell_size;
        }

        if (below_row >= 0 && below_row < height && check_col >= 0 && check_col < width) {
            if (lvl[below_row][check_col] == '-') {
                no_floor = true;
            }
        } else {
            no_floor = true;
        }

        if (wall_ahead || no_floor) {
            invisible_movingRight[pos] = !invisible_movingRight[pos];

            if (invisible_movingRight[pos]) {
                invisible_sprites[pos].setTexture(invisibleTextureRight);
            } else {
                invisible_sprites[pos].setTexture(invisibleTextureLeft);
            }
        } else {
            invisible_x[pos] = next_x;
        }

        if (rand() % 150 == 0) {
            invisible_movingRight[pos] = !invisible_movingRight[pos];

            if (invisible_movingRight[pos]) {
                invisible_sprites[pos].setTexture(invisibleTextureRight);
            } else {
                invisible_sprites[pos].setTexture(invisibleTextureLeft);
            }
        }

        if (rand() % 300 == 0 && !invisible_teleporting[pos]) {
            int current_platform_row = (int)(invisible_y[pos] + invisible_height) / cell_size;

            for (int levels_up = 1; levels_up <= 3; levels_up++) {
                int target_row = current_platform_row - levels_up * 2;

                if (target_row >= 1 && target_row < height) {
                    int col = (int)(invisible_x[pos] + invisible_width / 2) / cell_size;

                    if (col >= 0 && col < width && target_row >= 0) {
                        bool has_space = (lvl[target_row - 1][col] == '-');
                        bool has_floor = (lvl[target_row][col] == '#');

                        if (has_space && has_floor) {
                            invisible_target_y[pos] = (target_row * cell_size) - invisible_height;
                            invisible_teleporting[pos] = true;
                            invisible_anim_frame[pos] = 0;
                            invisible_anim_timer[pos] = 0;
                            break;
                        }
                    }
                }
            }
        }

        if (rand() % 300== 0 && !invisible_teleporting[pos]) {
            int current_platform_row = (int)(invisible_y[pos] + invisible_height) / cell_size;

            for (int levels_down = 1; levels_down <= 3; levels_down++) {
                int target_row = current_platform_row + levels_down * 2;

                if (target_row < height - 1) {
                    int col = (int)(invisible_x[pos] + invisible_width / 2) / cell_size;

                    if (col >= 0 && col < width && target_row < height) {
                        bool has_space = (lvl[target_row - 1][col] == '-');
                        bool has_floor = (lvl[target_row][col] == '#');

                        if (has_space && has_floor) {
                            invisible_target_y[pos] = (target_row * cell_size) - invisible_height;
                            invisible_teleporting[pos] = true;
                            invisible_anim_frame[pos] = 0;
                            invisible_anim_timer[pos] = 0;
                            break;
                        }
                    }
                }
            }
        }

        invisible_sprites[pos].setPosition(invisible_x[pos], invisible_y[pos]);
    }
}

bool check_player_invisible_collision(float player_x, float player_y, int PlayerWidth, int PlayerHeight,
                                      float invisible_x[], float invisible_y[], int num_invisible,
                                      bool enemy_active[], int invisible_start_index,
                                      bool enemy_stunned[], bool enemy_pulling[],
                                      bool invisible_teleporting[]) {
    int invisible_width = 96;
    int invisible_height = 102;

    for (int pos = 0; pos < num_invisible; pos++) {
        int enemy_index = invisible_start_index + pos;

        if (!enemy_active[enemy_index]) continue;
        if (enemy_stunned[enemy_index] || enemy_pulling[enemy_index]) continue;
        if (invisible_teleporting[pos]) continue;

        if (player_x < invisible_x[pos] + invisible_width &&
            player_x + PlayerWidth > invisible_x[pos] &&
            player_y < invisible_y[pos] + invisible_height &&
            player_y + PlayerHeight > invisible_y[pos]) {
            return true;
        }
    }
    return false;
}

bool check_player_chelnov_collision(float player_x, float player_y, int PlayerWidth, int PlayerHeight,
                                    float chelnov_x[], float chelnov_y[], int num_chelnovs,
                                    bool enemy_active[], int chelnov_start_index,
                                    bool enemy_stunned[], bool enemy_pulling[],
                                    bool chelnov_is_shooting[]) {
    int chelnov_width = 96;
    int chelnov_height = 102;

    for (int pos = 0; pos < num_chelnovs; pos++) {
        int enemy_index = chelnov_start_index + pos;

        if (!enemy_active[enemy_index]) continue;
        if (enemy_stunned[enemy_index] || enemy_pulling[enemy_index]) continue;

        if (chelnov_is_shooting[pos]) continue;

        if (player_x < chelnov_x[pos] + chelnov_width &&
            player_x + PlayerWidth > chelnov_x[pos] &&
            player_y < chelnov_y[pos] + chelnov_height &&
            player_y + PlayerHeight > chelnov_y[pos]) {
            return true;
        }
    }
    return false;
}

bool check_player_skeleton_collision(float player_x, float player_y, int PlayerWidth, int PlayerHeight,
                                      float skeleton_x[], float skeleton_y[], int num_skeletons,
                                      bool enemy_active[], int skeleton_start_index,
                                      bool enemy_stunned[], bool enemy_pulling[]) {
    int skeleton_width = 96;
    int skeleton_height = 102;

    for (int pos = 0; pos < num_skeletons; pos++) {
        int enemy_index = skeleton_start_index + pos;

        if (!enemy_active[enemy_index]) continue;
        if (enemy_stunned[enemy_index] || enemy_pulling[enemy_index]) continue;

        if (player_x < skeleton_x[pos] + skeleton_width &&
            player_x + PlayerWidth > skeleton_x[pos] &&
            player_y < skeleton_y[pos] + skeleton_height &&
            player_y + PlayerHeight > skeleton_y[pos]) {
            return true;
        }
    }
    return false;
}

bool check_player_ghost_collision(float player_x, float player_y, int PlayerWidth, int PlayerHeight,
                                   float ghost_x[], float ghost_y[], int num_ghosts,
                                   bool enemy_active[], int ghost_start_index,
                                   bool enemy_stunned[], bool enemy_pulling[]) {
    int ghost_width = 96;
    int ghost_height = 102;

    for (int pos = 0; pos < num_ghosts; pos++) {
        int enemy_index = ghost_start_index + pos;

        if (!enemy_active[enemy_index]) continue;
        if (enemy_stunned[enemy_index] || enemy_pulling[enemy_index]) continue;

        if (player_x < ghost_x[pos] + ghost_width &&
            player_x + PlayerWidth > ghost_x[pos] &&
            player_y < ghost_y[pos] + ghost_height &&
            player_y + PlayerHeight > ghost_y[pos]) {
            return true;
        }
    }
    return false;
}
void update_ghosts(float ghost_x[], float ghost_y[], float ghost_speed[], bool ghost_movingRight[],
                   Sprite ghost_sprites[], int num_ghosts, char** lvl, int height, int width, int cell_size,
                   bool enemy_active[], int ghost_start_index, bool enemy_stunned[], bool enemy_pulling[]) {
    
    const int ghost_width = 96;
    const int ghost_height = 102;
    const int grid_size = 64; 

    for (int pos = 0; pos < num_ghosts; pos++) {
        int enemy_index = ghost_start_index + pos;
        if (!enemy_active[enemy_index]) continue;

        if (enemy_stunned[enemy_index] || enemy_pulling[enemy_index]) {
            ghost_sprites[pos].setPosition(ghost_x[pos], ghost_y[pos]);
            continue;
        }

        // 1. BALANCED SPEED
        // 0.5f is a brisk walk; adjust this number up or down for the perfect feel
        float move_amount = ghost_speed[pos] * 0.5f; 
        float next_x = ghost_x[pos] + (ghost_movingRight[pos] ? move_amount : -move_amount);

        // 2. COLLISION DETECTION
        int check_col = (ghost_movingRight[pos]) 
                        ? (int)(next_x + ghost_width - 2) / grid_size  
                        : (int)(next_x + 2) / grid_size;               
        
        int mid_row = (int)(ghost_y[pos] + (ghost_height / 2)) / grid_size;
        int feet_row = (int)(ghost_y[pos] + ghost_height + 2) / grid_size;

        bool wall_ahead = false;
        bool no_floor = false;

        if (next_x < 0 || next_x + ghost_width >= width * grid_size) {
            wall_ahead = true;
        } else if (check_col >= 0 && check_col < width && mid_row >= 0 && mid_row < height) {
            if (lvl[mid_row][check_col] == '#') wall_ahead = true;
        }

        if (!wall_ahead && feet_row < height && check_col >= 0 && check_col < width) {
            if (lvl[feet_row][check_col] != '#') no_floor = true;
        }

        // 3. THE FLIP FIX (No more entering walls)
        if (wall_ahead || no_floor) {
            // SNAP: If moving right, snap to left side of wall. If left, snap to right side.
            if (ghost_movingRight[pos]) {
                ghost_x[pos] = (float)(check_col * grid_size) - ghost_width;
            } else {
                ghost_x[pos] = (float)((check_col + 1) * grid_size);
            }
            // TURN: Flip direction logic
            ghost_movingRight[pos] = !ghost_movingRight[pos];
        } else {
            // MOVE: Only apply movement if path is clear
            ghost_x[pos] = next_x;
        }

        // 4. VISUAL FLIP (Precise setScale)
        // We reset the origin to 0,0 to prevent "strange" jumping during the flip
        ghost_sprites[pos].setOrigin(0, 0);

        if (ghost_movingRight[pos]) {
            // Flip and offset so it doesn't jump to the side
            ghost_sprites[pos].setScale(-3.0f, 3.0f);
            ghost_sprites[pos].setPosition(ghost_x[pos] + ghost_width, ghost_y[pos]);
        } else {
            // Normal scale
            ghost_sprites[pos].setScale(3.0f, 3.0f);
            ghost_sprites[pos].setPosition(ghost_x[pos], ghost_y[pos]);
        }
    }
}
void update_score(int& total_score, int& high_score, int& current_combo, int& combo_timer,
                 char enemy_type, bool is_projectile_kill) {
    int base_points = 0;

    if (enemy_type == 'G') base_points = 50;
    else if (enemy_type == 'S') base_points = 75;
    else if (enemy_type == 'I') base_points = 150;
    else if (enemy_type == 'C') base_points = 200;

    if (is_projectile_kill) {
        base_points *= 2;
    }

    float combo_multiplier = 1.0f;
    if (current_combo >= 3 && current_combo <= 4) {
        combo_multiplier = 1.5f;
    } else if (current_combo >= 5) {
        combo_multiplier = 2.0f;
    }

    int final_points = (int)(base_points * combo_multiplier);
    total_score += final_points;

    if (total_score > high_score) {
        high_score = total_score;
    }

    combo_timer = 0;
}

void update_vacuum_animation(bool& vacuum_active, bool vacuum_was_pulling, int& vacuum_frame,
                             int& vacuum_timer, Sprite& VacuumSprite,
                             Texture& playerVacuumTexRight, Texture& playerVacuumTexLeft,
                             bool facingRight, float player_x, float player_y,
                             int PlayerWidth, int PlayerHeight, bool enemy_pulling[],
                             bool enemy_stunned[], int enemy_stun_frames[],
                             float vacuum_offset_x, float vacuum_offset_y,
                             int num_ghosts, int ghost_start_index,
                             int num_skeletons, int skeleton_start_index, bool skeleton_onGround[],
                             int num_chelnov, int chelnov_start_index, bool chelnov_onGround[],
                             int num_invisible, int invisible_start_index){

    if (vacuum_active) {
        if (facingRight) {
            VacuumSprite.setTexture(playerVacuumTexRight);
        } else {
            VacuumSprite.setTexture(playerVacuumTexLeft);
        }

        VacuumSprite.setScale(3, 3);

        float vacuumWidth = 96 * 3;
        float vacuumHeight = 96 * 3;
        float vac_x, vac_y;

        vac_y = player_y + (PlayerHeight / 2) - (vacuumHeight / 2) + 120;

        if (facingRight) {
            vac_x = player_x + PlayerWidth;
        } else {
            vac_x = player_x - PlayerWidth;
        }

        VacuumSprite.setPosition(vac_x, vac_y);
    }
else if (vacuum_was_pulling) {

    for (int pos = 0; pos < num_ghosts; pos++) {
        int enemy_index = ghost_start_index + pos;
        if (enemy_pulling[enemy_index]) {

            enemy_stunned[enemy_index] = true;
            enemy_pulling[enemy_index] = false;
            enemy_stun_frames[enemy_index] = 30;
        }
    }

    for (int pos = 0; pos < num_skeletons; pos++) {
        int enemy_index = skeleton_start_index + pos;
        if (enemy_pulling[enemy_index]) {

            if (skeleton_onGround[pos]) {
                enemy_stunned[enemy_index] = true;
                enemy_stun_frames[enemy_index] = 30;
            }
            enemy_pulling[enemy_index] = false;
        }
    }

    for (int pos = 0; pos < num_chelnov; pos++) {
        int enemy_index = chelnov_start_index + pos;
        if (enemy_pulling[enemy_index]) {

            if (chelnov_onGround[pos]) {
                enemy_stunned[enemy_index] = true;
                enemy_stun_frames[enemy_index] = 30;
            }
            enemy_pulling[enemy_index] = false;
        }
    }

    for (int pos = 0; pos < num_invisible; pos++) {
        int enemy_index = invisible_start_index + pos;
        if (enemy_pulling[enemy_index]) {

            enemy_stunned[enemy_index] = true;
            enemy_stun_frames[enemy_index] = 30;
            enemy_pulling[enemy_index] = false;
        }
    }
}
}

void vacuum_pull_enemies(bool vacuum_active, int& enemies_in_bag, int max_bag_capacity,
                        float player_x, float player_y, int PlayerWidth, int PlayerHeight,
                        bool facingRight, int vacuum_range, int cell_size,int vacuum_direction,
                        float ghost_x[], float ghost_y[], int num_ghosts, int ghost_start_index,
                        float skeleton_x[], float skeleton_y[], int num_skeletons, int skeleton_start_index,
                        float chelnov_x[], float chelnov_y[], int num_chelnov, int chelnov_start_index,
                        bool enemy_active[], bool enemy_stunned[], bool enemy_pulling[],
                        int enemy_stun_frames[], int enemy_pull_frames[],
                        int bagged_enemies[], char bagged_enemy_types[],
                        float vacuum_offset_x, float vacuum_offset_y,
                        float invisible_x[], float invisible_y[], int num_invisible, int invisible_start_index,
                        bool invisible_teleporting[],
                        int& total_score, int& high_score, int& current_combo, int& combo_timer){
    if (!vacuum_active || enemies_in_bag >= max_bag_capacity) return;

float vacuum_center_x = player_x + PlayerWidth / 2;
float vacuum_center_y = player_y + PlayerHeight / 2;

int player_platform_row = (int)(player_y + PlayerHeight) / cell_size;

for (int pos = 0; pos < num_ghosts && enemies_in_bag < max_bag_capacity; pos++) {
    int enemy_index = ghost_start_index + pos;
    if (!enemy_active[enemy_index]) continue;

    float enemy_center_x = ghost_x[pos] + 48.0f;
    float enemy_center_y = ghost_y[pos] + 51.0f;

    float dx = 0, dy = 0;
    bool in_range = false;

    if (vacuum_direction == 0) {
        int enemy_platform_row = (int)(ghost_y[pos] + 102) / cell_size;
        if (enemy_platform_row != player_platform_row) continue;

        dx = enemy_center_x - vacuum_center_x;
        float distance = abs(dx);
        bool in_front = facingRight ? (dx > 0) : (dx < 0);
        in_range = (distance <= vacuum_range && in_front);
    }
    else if (vacuum_direction == 1) {
        dy = enemy_center_y - vacuum_center_y;
        float distance = abs(dy);
        in_range = (distance <= vacuum_range && dy < 0);
        dx = 0;
    }
    else if (vacuum_direction == 2) {
        dy = enemy_center_y - vacuum_center_y;
        float distance = abs(dy);
        in_range = (distance <= vacuum_range && dy > 0);
        dx = 0;
    }
    else if (vacuum_direction == 3) {
        int enemy_platform_row = (int)(ghost_y[pos] + 102) / cell_size;
        if (enemy_platform_row != player_platform_row) continue;

        dx = enemy_center_x - vacuum_center_x;
        float distance = abs(dx);
        bool behind = facingRight ? (dx < 0) : (dx > 0);
        in_range = (distance <= vacuum_range && behind);
    }

    float distance = sqrt(dx * dx + dy * dy);

    if (in_range) {
        if (!enemy_pulling[enemy_index]) {
            enemy_stunned[enemy_index] = true;
            enemy_pulling[enemy_index] = true;
            enemy_stun_frames[enemy_index] = 0;
            enemy_pull_frames[enemy_index] = 0;
        }

        enemy_pull_frames[enemy_index]++;
        float pull_speed = 3.0f + (enemy_pull_frames[enemy_index] / 10.0f);

        if (distance > 0) {
            float pull_dx = -dx / distance * pull_speed;
            float pull_dy = -dy / distance * pull_speed;
            ghost_x[pos] += pull_dx;
            ghost_y[pos] += pull_dy;
        }

        if (distance < 30.0f) {
            enemy_active[enemy_index] = false;
            ghost_x[pos] = -9999.0f;
            ghost_y[pos] = -9999.0f;

            bagged_enemies[enemies_in_bag] = enemy_index;
            bagged_enemy_types[enemies_in_bag] = 'G';
            enemies_in_bag++;

             current_combo++;
            update_score(total_score, high_score, current_combo, combo_timer, 'G', false);

            enemy_stunned[enemy_index] = false;
            enemy_pulling[enemy_index] = false;
            enemy_stun_frames[enemy_index] = 0;
            enemy_pull_frames[enemy_index] = 0;
        }
    }
}

for (int pos = 0; pos < num_skeletons && enemies_in_bag < max_bag_capacity; pos++) {
    int enemy_index = skeleton_start_index + pos;
    if (!enemy_active[enemy_index]) continue;

    float enemy_center_x = skeleton_x[pos] + 48.0f;
    float enemy_center_y = skeleton_y[pos] + 51.0f;

    float dx = 0, dy = 0;
    bool in_range = false;

    if (vacuum_direction == 0) {
        int enemy_platform_row = (int)(skeleton_y[pos] + 102) / cell_size;
        if (enemy_platform_row != player_platform_row) continue;

        dx = enemy_center_x - vacuum_center_x;
        float distance = abs(dx);
        bool in_front = facingRight ? (dx > 0) : (dx < 0);
        in_range = (distance <= vacuum_range && in_front);
    }
    else if (vacuum_direction == 1) {
        dy = enemy_center_y - vacuum_center_y;
        float distance = abs(dy);
        in_range = (distance <= vacuum_range && dy < 0);
        dx = 0;
    }
    else if (vacuum_direction == 2) {
        dy = enemy_center_y - vacuum_center_y;
        float distance = abs(dy);
        in_range = (distance <= vacuum_range && dy > 0);
        dx = 0;
    }
    else if (vacuum_direction == 3) {
        int enemy_platform_row = (int)(skeleton_y[pos] + 102) / cell_size;
        if (enemy_platform_row != player_platform_row) continue;

        dx = enemy_center_x - vacuum_center_x;
        float distance = abs(dx);
        bool behind = facingRight ? (dx < 0) : (dx > 0);
        in_range = (distance <= vacuum_range && behind);
    }

    float distance = sqrt(dx * dx + dy * dy);

    if (in_range) {
        if (!enemy_pulling[enemy_index]) {
            enemy_stunned[enemy_index] = true;
            enemy_pulling[enemy_index] = true;
            enemy_stun_frames[enemy_index] = 0;
            enemy_pull_frames[enemy_index] = 0;
        }

        enemy_pull_frames[enemy_index]++;
        float pull_speed = 3.0f + (enemy_pull_frames[enemy_index] / 10.0f);

        if (distance > 0) {
            float pull_dx = -dx / distance * pull_speed;
            float pull_dy = -dy / distance * pull_speed;
            skeleton_x[pos] += pull_dx;
            skeleton_y[pos] += pull_dy;
        }

        if (distance < 30.0f) {
            enemy_active[enemy_index] = false;
            skeleton_x[pos] = -9999.0f;
            skeleton_y[pos] = -9999.0f;

            bagged_enemies[enemies_in_bag] = enemy_index;
            bagged_enemy_types[enemies_in_bag] = 'S';
            enemies_in_bag++;
             current_combo++;
            update_score(total_score, high_score, current_combo, combo_timer, 'S', false);
            enemy_stunned[enemy_index] = false;
            enemy_pulling[enemy_index] = false;
            enemy_stun_frames[enemy_index] = 0;
            enemy_pull_frames[enemy_index] = 0;
        }
    }
}

for (int pos = 0; pos < num_chelnov && enemies_in_bag < max_bag_capacity; pos++) {
    int enemy_index = chelnov_start_index + pos;
    if (!enemy_active[enemy_index]) continue;

    float enemy_center_x = chelnov_x[pos] + 48.0f;
    float enemy_center_y = chelnov_y[pos] + 51.0f;

    float dx = 0, dy = 0;
    bool in_range = false;

    if (vacuum_direction == 0) {
        int enemy_platform_row = (int)(chelnov_y[pos] + 102) / cell_size;
        if (enemy_platform_row != player_platform_row) continue;

        dx = enemy_center_x - vacuum_center_x;
        float distance = abs(dx);
        bool in_front = facingRight ? (dx > 0) : (dx < 0);
        in_range = (distance <= vacuum_range && in_front);
    }
    else if (vacuum_direction == 1) {
        dy = enemy_center_y - vacuum_center_y;
        float distance = abs(dy);
        in_range = (distance <= vacuum_range && dy < 0);
        dx = 0;
    }
    else if (vacuum_direction == 2) {
        dy = enemy_center_y - vacuum_center_y;
        float distance = abs(dy);
        in_range = (distance <= vacuum_range && dy > 0);
        dx = 0;
    }
    else if (vacuum_direction == 3) {
        int enemy_platform_row = (int)(chelnov_y[pos] + 102) / cell_size;
        if (enemy_platform_row != player_platform_row) continue;

        dx = enemy_center_x - vacuum_center_x;
        float distance = abs(dx);
        bool behind = facingRight ? (dx < 0) : (dx > 0);
        in_range = (distance <= vacuum_range && behind);
    }

    float distance = sqrt(dx * dx + dy * dy);

    if (in_range) {
        if (!enemy_pulling[enemy_index]) {
            enemy_stunned[enemy_index] = true;
            enemy_pulling[enemy_index] = true;
            enemy_stun_frames[enemy_index] = 0;
            enemy_pull_frames[enemy_index] = 0;
        }

        enemy_pull_frames[enemy_index]++;
        float pull_speed = 3.0f + (enemy_pull_frames[enemy_index] / 10.0f);

        if (distance > 0) {
            float pull_dx = -dx / distance * pull_speed;
            float pull_dy = -dy / distance * pull_speed;
            chelnov_x[pos] += pull_dx;
            chelnov_y[pos] += pull_dy;
        }

        if (distance < 30.0f) {
            enemy_active[enemy_index] = false;
            chelnov_x[pos] = -9999.0f;
            chelnov_y[pos] = -9999.0f;

            bagged_enemies[enemies_in_bag] = enemy_index;
            bagged_enemy_types[enemies_in_bag] = 'C';
            enemies_in_bag++;
             current_combo++;
    update_score(total_score, high_score, current_combo, combo_timer, 'C', false);

            enemy_stunned[enemy_index] = false;
            enemy_pulling[enemy_index] = false;
            enemy_stun_frames[enemy_index] = 0;
            enemy_pull_frames[enemy_index] = 0;
        }
    }
}

for (int pos = 0; pos < num_invisible && enemies_in_bag < max_bag_capacity; pos++) {
    int enemy_index = invisible_start_index + pos;
    if (!enemy_active[enemy_index]) continue;
    if (invisible_teleporting[pos]) continue;

    float enemy_center_x = invisible_x[pos] + 48.0f;
    float enemy_center_y = invisible_y[pos] + 51.0f;

    float dx = 0, dy = 0;
    bool in_range = false;

    if (vacuum_direction == 0) {
        int enemy_platform_row = (int)(invisible_y[pos] + 102) / cell_size;
        if (enemy_platform_row != player_platform_row) continue;

        dx = enemy_center_x - vacuum_center_x;
        float distance = abs(dx);
        bool in_front = facingRight ? (dx > 0) : (dx < 0);
        in_range = (distance <= vacuum_range && in_front);
    }
    else if (vacuum_direction == 1) {
        dy = enemy_center_y - vacuum_center_y;
        float distance = abs(dy);
        in_range = (distance <= vacuum_range && dy < 0);
        dx = 0;
    }
    else if (vacuum_direction == 2) {
        dy = enemy_center_y - vacuum_center_y;
        float distance = abs(dy);
        in_range = (distance <= vacuum_range && dy > 0);
        dx = 0;
    }
    else if (vacuum_direction == 3) {
        int enemy_platform_row = (int)(invisible_y[pos] + 102) / cell_size;
        if (enemy_platform_row != player_platform_row) continue;

        dx = enemy_center_x - vacuum_center_x;
        float distance = abs(dx);
        bool behind = facingRight ? (dx < 0) : (dx > 0);
        in_range = (distance <= vacuum_range && behind);
    }

    float distance = sqrt(dx * dx + dy * dy);

    if (in_range) {
        if (!enemy_pulling[enemy_index]) {
            enemy_stunned[enemy_index] = true;
            enemy_pulling[enemy_index] = true;
            enemy_stun_frames[enemy_index] = 0;
            enemy_pull_frames[enemy_index] = 0;
        }

        enemy_pull_frames[enemy_index]++;
        float pull_speed = 3.0f + (enemy_pull_frames[enemy_index] / 10.0f);

        if (distance > 0) {
            float pull_dx = -dx / distance * pull_speed;
            float pull_dy = -dy / distance * pull_speed;
            invisible_x[pos] += pull_dx;
            invisible_y[pos] += pull_dy;
        }

        if (distance < 30.0f) {
            enemy_active[enemy_index] = false;
            invisible_x[pos] = -9999.0f;
            invisible_y[pos] = -9999.0f;

            bagged_enemies[enemies_in_bag] = enemy_index;
            bagged_enemy_types[enemies_in_bag] = 'I';
            enemies_in_bag++;
             current_combo++;
    update_score(total_score, high_score, current_combo, combo_timer, 'I', false);
            enemy_stunned[enemy_index] = false;
            enemy_pulling[enemy_index] = false;
            enemy_stun_frames[enemy_index] = 0;
            enemy_pull_frames[enemy_index] = 0;
        }
    }
}
}
void shoot_bullet(int& enemies_in_bag, int& shoot_cooldown_frames,
                 float bullet_x[], float bullet_y[], float bullet_speed_x[], float bullet_speed_y[],
                 bool bullet_active[], char bullet_type[], CircleShape bullet_shapes[],
                 int max_bullets, float player_x, float player_y, int PlayerWidth,
                 bool facingRight, int bagged_enemies[], char bagged_enemy_types[],
                 Texture& ghostTexture, Texture& skeletonTextureRight, Texture& chelnovTextureRight) {
    
    if (shoot_cooldown_frames > 0 || enemies_in_bag <= 0) return;

    for (int pos = 0; pos < max_bullets; pos++) {
        if (!bullet_active[pos]) {
            bullet_active[pos] = true;
            const float radius = 30.0f;

            bullet_x[pos] = facingRight ? (player_x + PlayerWidth + 10) : (player_x - (radius * 2) - 10);
            bullet_y[pos] = player_y + 30;
            bullet_speed_x[pos] = facingRight ? 10.0f : -10.0f; // Space bullets move faster!
            bullet_speed_y[pos] = -1.5f; // Slight floaty pop

            int last_index = enemies_in_bag - 1;
            bullet_type[pos] = bagged_enemy_types[last_index];
            enemies_in_bag--;

            bullet_shapes[pos].setRadius(radius);
            bullet_shapes[pos].setOrigin(radius, radius); // Origin to center for "Space Spin"

            // Space Vibe: Add a glowing neon outline and a tint
            bullet_shapes[pos].setOutlineThickness(3);
            bullet_shapes[pos].setOutlineColor(Color(0, 255, 255, 200)); // Neon Cyan
            bullet_shapes[pos].setFillColor(Color(255, 255, 255, 180)); // Slight transparency

            if (bullet_type[pos] == 'G') bullet_shapes[pos].setTexture(&ghostTexture);
            else if (bullet_type[pos] == 'S') bullet_shapes[pos].setTexture(&skeletonTextureRight);
            else if (bullet_type[pos] == 'C') bullet_shapes[pos].setTexture(&chelnovTextureRight);

            shoot_cooldown_frames = 15;
            break;
        }
    }
}
void update_bullets(float bullet_x[], float bullet_y[], float bullet_speed_x[], float bullet_speed_y[],
                   bool bullet_active[], CircleShape bullet_shapes[], int max_bullets,
                   char** lvl, int height, int width, int cell_size, int screen_x, int screen_y,
                   float ghost_x[], float ghost_y[], int num_ghosts, int ghost_start_index,
                   float skeleton_x[], float skeleton_y[], int num_skeletons, int skeleton_start_index,
                   float chelnov_x[], float chelnov_y[], int num_chelnov, int chelnov_start_index,
                   bool enemy_active[], bool& pot_active, bool& pot_destroyed,
                   int pot_height, int pot_width, int& pot_health, float pot_x, float pot_y,
                   int& total_score, int& high_score, int& current_combo, int& combo_timer) {

    const float radius = 30.0f;
    const float b_w = 60.0f; // Simplified width of the circular bullet for logic
    const float b_h = 60.0f; // Simplified height of the circular bullet for logic

    for (int pos = 0; pos < max_bullets; pos++) {
        if (!bullet_active[pos]) continue;

        float old_x = bullet_x[pos];
        float old_y = bullet_y[pos];

        // --- 1. MOVEMENT & PHYSICS ---
        bullet_x[pos] += bullet_speed_x[pos];
        bullet_speed_y[pos] += 0.8f;
        bullet_y[pos] += bullet_speed_y[pos];
        // Inside the update_bullets loop, after moving bullet_x and bullet_y:
if (bullet_active[pos]) {
    // Rotation speed based on movement direction
    float spin = (bullet_speed_x[pos] > 0) ? 15.0f : -15.0f;
    bullet_shapes[pos].rotate(spin);
    
    // Pulse effect: slightly change the outline color over time for a "flicker"
    int alpha = 150 + (rand() % 105); 
    bullet_shapes[pos].setOutlineColor(Color(0, 255, 255, alpha));
}

        // --- 2. SCREEN & BOTTOM-FLOOR VANISH LOGIC ---
        if (bullet_y[pos] + b_h > screen_y) {
            bullet_active[pos] = false;
            continue;
        }

        if (bullet_x[pos] < 0) {
            bullet_x[pos] = 0;
            bullet_speed_x[pos] = -bullet_speed_x[pos];
            if (bullet_y[pos] + b_h >= screen_y - 64) { bullet_active[pos] = false; continue; }
        }
        if (bullet_x[pos] + b_w > screen_x) {
            bullet_x[pos] = screen_x - b_w;
            bullet_speed_x[pos] = -bullet_speed_x[pos];
            if (bullet_y[pos] + b_h >= screen_y - 64) { bullet_active[pos] = false; continue; }
        }

        // --- 3. TILE COLLISION ---
        int bullet_bottom_row = (int)(bullet_y[pos] + b_h) / cell_size;
        int bullet_center_col = (int)(bullet_x[pos] + radius) / cell_size;
        int bullet_left_col = (int)(bullet_x[pos]) / cell_size;
        int bullet_right_col = (int)(bullet_x[pos] + b_w) / cell_size;

        bool on_ground = false;
        if (bullet_speed_y[pos] > 0) {
            if (bullet_bottom_row >= 0 && bullet_bottom_row < height && bullet_center_col < width) {
                if (lvl[bullet_bottom_row][bullet_left_col] == '#' || 
                    lvl[bullet_bottom_row][bullet_center_col] == '#' || 
                    lvl[bullet_bottom_row][bullet_right_col] == '#') {
                    
                    bullet_y[pos] = (bullet_bottom_row * cell_size) - b_h;
                    bullet_speed_y[pos] = 0;
                    on_ground = true;

                    // Vanish at bottom-most corners
                    if (bullet_bottom_row >= height - 1) {
                        if (bullet_x[pos] <= 64 || bullet_x[pos] + b_w >= screen_x - 64) {
                            bullet_active[pos] = false;
                            continue;
                        }
                    }
                }
            }
        }

        // Horizontal Wall Collision
        int waist_row = (int)(bullet_y[pos] + radius) / cell_size;
        if (waist_row >= 0 && waist_row < height) {
            if (bullet_speed_x[pos] > 0 && bullet_right_col < width && lvl[waist_row][bullet_right_col] == '#') {
                bullet_x[pos] = old_x;
                bullet_speed_x[pos] = -bullet_speed_x[pos];
            } else if (bullet_speed_x[pos] < 0 && bullet_left_col >= 0 && lvl[waist_row][bullet_left_col] == '#') {
                bullet_x[pos] = old_x;
                bullet_speed_x[pos] = -bullet_speed_x[pos];
            }
        }

        // --- 4. ENEMY KILL LOGIC (RE-ADDED) ---
        bool bullet_hit_enemy = false;

        // Pot Hit
        if (pot_active && !pot_destroyed) {
            if (bullet_x[pos] < pot_x + pot_width && bullet_x[pos] + b_w > pot_x &&
                bullet_y[pos] < pot_y + pot_height && bullet_y[pos] + b_h > pot_y) {
                pot_health--;
                bullet_active[pos] = false;
                if (pot_health <= 0) pot_destroyed = true;
                continue;
            }
        }

        // Ghost Hit
        for (int i = 0; i < num_ghosts; i++) {
            int idx = ghost_start_index + i;
            if (!enemy_active[idx]) continue;
            if (bullet_x[pos] < ghost_x[i] + 96 && bullet_x[pos] + b_w > ghost_x[i] &&
                bullet_y[pos] < ghost_y[i] + 102 && bullet_y[pos] + b_h > ghost_y[i]) {
                enemy_active[idx] = false;
                current_combo++;
                update_score(total_score, high_score, current_combo, combo_timer, 'G', true);
                bullet_active[pos] = false;
                bullet_hit_enemy = true;
                break;
            }
        }
        if (bullet_hit_enemy) continue;

        // Skeleton Hit
        for (int i = 0; i < num_skeletons; i++) {
            int idx = skeleton_start_index + i;
            if (!enemy_active[idx]) continue;
            if (bullet_x[pos] < skeleton_x[i] + 96 && bullet_x[pos] + b_w > skeleton_x[i] &&
                bullet_y[pos] < skeleton_y[i] + 102 && bullet_y[pos] + b_h > skeleton_y[i]) {
                enemy_active[idx] = false;
                current_combo++;
                update_score(total_score, high_score, current_combo, combo_timer, 'S', true);
                bullet_active[pos] = false;
                bullet_hit_enemy = true;
                break;
            }
        }
        if (bullet_hit_enemy) continue;

        // Chelnov Hit
        for (int i = 0; i < num_chelnov; i++) {
            int idx = chelnov_start_index + i;
            if (!enemy_active[idx]) continue;
            if (bullet_x[pos] < chelnov_x[i] + 96 && bullet_x[pos] + b_w > chelnov_x[i] &&
                bullet_y[pos] < chelnov_y[i] + 102 && bullet_y[pos] + b_h > chelnov_y[i]) {
                enemy_active[idx] = false;
                current_combo++;
                update_score(total_score, high_score, current_combo, combo_timer, 'C', true);
                bullet_active[pos] = false;
                break;
            }
        }
    }
}
void draw_bullets(RenderWindow& window, bool bullet_active[], CircleShape bullet_shapes[],
                 float bullet_x[], float bullet_y[], int max_bullets) {
    for (int pos = 0; pos < max_bullets; pos++) {
        if (bullet_active[pos]) {
            bullet_shapes[pos].setPosition(bullet_x[pos] + 30.0f, bullet_y[pos] + 30.0f);
            window.draw(bullet_shapes[pos]);
        }
    }
}
void update_stun_frames(bool enemy_stunned[], int enemy_stun_frames[]) {
    for (int pos = 0; pos < 60; pos++) {
        if (enemy_stun_frames[pos] > 0) {
            enemy_stun_frames[pos]--;
            if (enemy_stun_frames[pos] == 0) {
                enemy_stunned[pos] = false;
            }
        }
    }
}
int showmenu(RenderWindow& window) {
    Texture bgTex;
    bgTex.loadFromFile("Data/bg1.png");
    Sprite bg(bgTex);

    Font font;
    font.loadFromFile("Data/Arial.ttf");

    SoundBuffer hoverBuffer;
    Sound hoverSound;
    if (hoverBuffer.loadFromFile("Data/click.wav")) {
        hoverSound.setBuffer(hoverBuffer);
    }

    auto styleBtn = [&](RectangleShape& b, Text& t, string str, float y) {
        b.setSize(Vector2f(450, 65));
        b.setFillColor(Color(20, 20, 50, 160)); 
        b.setOutlineThickness(2);
        b.setOutlineColor(Color(0, 255, 255));
        
        // KEEPING YOUR SPECIFIC ALIGNMENT OFFSETS
        b.setOrigin(b.getSize().x / 2.0f, (b.getSize().y / 2.0f) - 100);
        b.setPosition(1136 / 2.0f, y);

        t.setFont(font);
        t.setString(str);
        t.setCharacterSize(26);
        t.setFillColor(Color(0, 255, 255));

        FloatRect tr = t.getLocalBounds();
        t.setOrigin(tr.left + tr.width / 2.0f, (tr.top + tr.height / 2.0f) - 100);
        t.setPosition(b.getPosition()); 
    };

    RectangleShape startBtn, instrBtn, levelBtn, L1Btn, L2Btn, backBtn;
    Text startText, instrText, levelText, L1Text, L2Text, backText;

    styleBtn(startBtn, startText, "LAUNCH MISSION", 350);
    styleBtn(instrBtn, instrText, "MISSION BRIEFING", 450);
    styleBtn(levelBtn, levelText, "SECTOR SELECTION", 550);
    
    styleBtn(L1Btn, L1Text, "SECTOR 1: THE BEGINNING", 350);
    styleBtn(L2Btn, L2Text, "SECTOR 2: THE DEEP", 450);
    styleBtn(backBtn, backText, "RETURN TO HUB", 620);

    enum State { Main, Instructions, LevelSelect };
    State currentState = Main;
    bool wasHovering = false;

    while (window.isOpen()) {
        Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));
        Event ev;

        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) window.close();

            if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                if (currentState == Main) {
                    if (startBtn.getGlobalBounds().contains(mouse)) return 1; 
                    if (instrBtn.getGlobalBounds().contains(mouse)) currentState = Instructions;
                    if (levelBtn.getGlobalBounds().contains(mouse)) currentState = LevelSelect;
                } 
                else if (currentState == LevelSelect) {
                    if (L1Btn.getGlobalBounds().contains(mouse)) return 1; 
                    if (L2Btn.getGlobalBounds().contains(mouse)) return 2; 
                    if (backBtn.getGlobalBounds().contains(mouse)) currentState = Main;
                } 
                else if (currentState == Instructions) {
                    if (backBtn.getGlobalBounds().contains(mouse)) currentState = Main;
                }
            }
        }

        auto handleHover = [&](RectangleShape& b, Text& t) {
            bool hovering = b.getGlobalBounds().contains(mouse);
            if (hovering) {
                b.setOutlineColor(Color::White);
                t.setFillColor(Color::White);
            } else {
                b.setOutlineColor(Color(0, 255, 255));
                t.setFillColor(Color(0, 255, 255));
            }
            return hovering;
        };

        bool anyHover = false;
        window.clear();
        window.draw(bg);

        if (currentState == Main) {
            if (handleHover(startBtn, startText)) anyHover = true;
            if (handleHover(instrBtn, instrText)) anyHover = true;
            if (handleHover(levelBtn, levelText)) anyHover = true;
            window.draw(startBtn); window.draw(startText);
            window.draw(instrBtn); window.draw(instrText);
            window.draw(levelBtn); window.draw(levelText);
        } 
        else if (currentState == LevelSelect) {
            if (handleHover(L1Btn, L1Text)) anyHover = true;
            if (handleHover(L2Btn, L2Text)) anyHover = true;
            if (handleHover(backBtn, backText)) anyHover = true;
            window.draw(L1Btn); window.draw(L1Text);
            window.draw(L2Btn); window.draw(L2Text);
            window.draw(backBtn); window.draw(backText);
        } 
        else if (currentState == Instructions) {
            handleHover(backBtn, backText);
            
            // Styled Instruction Box
            RectangleShape box(Vector2f(850, 400));
            box.setFillColor(Color(10, 10, 30, 230));
            box.setOutlineThickness(3);
            box.setOutlineColor(Color(0, 255, 255));
            box.setOrigin(425, 200);
            box.setPosition(1136 / 2.0f, 380);
            
            Text info("--- PILOT MANUAL ---\n\n"
                     "> THRUSTERS:  Use [W,A,S,D] to move through space.\n"
                     "> NEUTRON VAC: Press [C] to stun and capture aliens.\n"
                     "> MISSION:     Clear all hostiles to advance sectors.\n"
                     "> REPAIRS:     Lives reset to 3 upon sector completion.\n"
                     "> WARNING:    Direct contact causes ship hull damage!", font, 22);
            info.setFillColor(Color::Cyan);
            FloatRect ir = info.getLocalBounds();
            info.setOrigin(ir.left + ir.width / 2.0f, ir.top + ir.height / 2.0f);
            info.setPosition(box.getPosition());
            
            window.draw(box);
            window.draw(info);
            window.draw(backBtn); window.draw(backText);
        }

        if (anyHover && !wasHovering) hoverSound.play();
        wasHovering = anyHover;

        window.display();
    }
    return 0;
}
void drawStatBar(RenderWindow& window, Vector2f pos, string label, int value, Font& font, Color barColor) {
    // Label
    Text text(label, font, 18);
    text.setPosition(pos.x, pos.y - 25);
    window.draw(text);

    // Background of the bar (Empty)
    RectangleShape barBg(Vector2f(200, 15));
    barBg.setFillColor(Color(50, 50, 50));
    barBg.setPosition(pos);
    window.draw(barBg);

    // Foreground of the bar (Filled)
    RectangleShape barFill(Vector2f(value * 2.0f, 15)); // value is 0-100
    barFill.setFillColor(barColor);
    barFill.setPosition(pos);
    window.draw(barFill);
}

int selectedplayer(RenderWindow& window, Texture& selectedPlayerTextureRight, Texture& selectedPlayerTextureLeft){
    Texture p1Tex, p2Tex;
    p1Tex.loadFromFile("Data/rplayer.png");
    p2Tex.loadFromFile("Data/y.png");
    
    // Pixel-perfect settings
    p1Tex.setSmooth(false); 
    p2Tex.setSmooth(false);

    Font font;
    font.loadFromFile("Data/qe.ttf"); 

    // Character Sprites
    Sprite p1(p1Tex), p2(p2Tex);
    p1.setOrigin(p1.getLocalBounds().width / 2, p1.getLocalBounds().height / 2);
    p2.setOrigin(p2.getLocalBounds().width / 2, p2.getLocalBounds().height / 2);
    
    // Fixed Alignment Positions (Center-balanced)
    float screen_x = 1136.0f;
    float screen_y = 896.0f;
    p1.setPosition(screen_x * 0.28f, 448.0f);
    p2.setPosition(screen_x * 0.72f, 448.0f);

    CircleShape pedestal(100, 6);
    pedestal.setOrigin(100, 100);
    pedestal.setScale(2.5f, 0.7f);

    Clock guiClock;

    while (window.isOpen()) {
        Vector2f mPos = window.mapPixelToCoords(Mouse::getPosition(window));
        float time = guiClock.getElapsedTime().asSeconds();
        Event ev;

        while (window.pollEvent(ev)) {
            if (ev.type == Event::Closed) window.close();
            if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                if (mPos.x < screen_x / 2) {
                    selectedPlayerTextureRight.loadFromFile("Data/RPGwalk.png");
                    selectedPlayerTextureLeft.loadFromFile("Data/PGwalk.png");
                    return 1;
                }
                if (mPos.x >= screen_x / 2) {
                    selectedPlayerTextureRight.loadFromFile("Data/RPYwalk.png");
                    selectedPlayerTextureLeft.loadFromFile("Data/PYwalk.png");
                    return 2;
                }
            }
        }

        window.clear(Color(15, 15, 25)); 
        for (int i = 0; i < screen_y; i += 40) {
            RectangleShape line(Vector2f(screen_x, 1));
            line.setFillColor(Color(255, 255, 255, 15)); 
            line.setPosition(0, i);
            window.draw(line);
        }

        bool hoverL = (mPos.x < screen_x / 2);
        bool hoverR = !hoverL;
        float bob = sin(time * 4.0f) * 10.0f;

        pedestal.setPosition(p1.getPosition().x, 448 + 130);
        pedestal.setFillColor(Color(0, 255, 0, 20));
        pedestal.setOutlineColor(hoverL ? Color::Green : Color(60, 60, 60));
        pedestal.setOutlineThickness(2);
        window.draw(pedestal);

        p1.setScale(hoverL ? 6.0f : 5.0f, hoverL ? 6.0f : 5.0f);
        p1.setColor(hoverL ? Color::White : Color(100, 100, 100));
        p1.setPosition(screen_x * 0.28f, 448 + (hoverL ? bob : 0));
        window.draw(p1);

        pedestal.setPosition(p2.getPosition().x, 448 + 130);
        pedestal.setFillColor(Color(255, 255, 0, 10));
        pedestal.setOutlineColor(hoverR ? Color::Yellow : Color(60, 60, 60));
        window.draw(pedestal);

        p2.setScale(hoverR ? 6.0f : 5.0f, hoverR ? 6.0f : 5.0f);
        p2.setColor(hoverR ? Color::White : Color(100, 100, 100));
        p2.setPosition(screen_x * 0.72f, 448 + (hoverR ? bob : 0));
        window.draw(p2);

        RectangleShape card(Vector2f(340, 240));
        card.setFillColor(Color(0, 0, 0, 220));
        card.setOutlineThickness(2);
        card.setOutlineColor(hoverL ? Color::Green : Color::Yellow);
        card.setPosition(hoverL ? 60 : screen_x - 400, 580);
        window.draw(card);

        Text name(hoverL ? "COMMANDO" : "STRIKER", font, 30);
        name.setPosition(card.getPosition().x + 20, card.getPosition().y + 10);
        name.setFillColor(hoverL ? Color::Green : Color::Yellow);
        window.draw(name);

        Text h1("POWER LEVEL", font, 14);
        h1.setPosition(card.getPosition().x + 20, card.getPosition().y + 70);
        window.draw(h1);
        
        RectangleShape pBar(Vector2f(hoverL ? 250 : 150, 12));
        pBar.setPosition(card.getPosition().x + 20, card.getPosition().y + 95);
        pBar.setFillColor(Color(255, 50, 50));
        window.draw(pBar);

        Text h2("AGILITY / SPEED", font, 14);
        h2.setPosition(card.getPosition().x + 20, card.getPosition().y + 135);
        window.draw(h2);

        RectangleShape sBar(Vector2f(hoverL ? 120 : 280, 12));
        sBar.setPosition(card.getPosition().x + 20, card.getPosition().y + 160);
        sBar.setFillColor(Color(50, 255, 255));
        window.draw(sBar);

        Text title("CHOOSE YOUR HERO", font, 50);
        title.setOrigin(title.getGlobalBounds().width / 2, 0);
        title.setPosition(screen_x / 2, 40);
        title.setFillColor(Color(255, 255, 255, 200 + (int)(55 * sin(time * 4))));
        window.draw(title);

        window.display();
    }
    return 0;
}
int main()
{
    srand(time(0));
	RenderWindow window(VideoMode(screen_x, screen_y), "Tumble-POP", Style::Resize);
   int choice = showmenu(window); 
    if (choice == 0) return 0; // Exit if window closed

    Texture PlayerTextureRight;
    Texture PlayerTextureLeft;
    int selectedplayernum = selectedplayer(window, PlayerTextureRight, PlayerTextureLeft);
    // --- ADD THIS AT THE TOP OF MAIN ---
// Update your loading block to this:
Music gameMusic;
if (!gameMusic.openFromFile("Data/game.wav")) {
    std::cout << "ERROR: Could not find Data/game.wav!" << std::endl;
} else {
    std::cout << "Success: Music loaded!" << std::endl;
    gameMusic.setVolume(50.0f); // Set it to 50 for testing
    gameMusic.setLoop(true);
    gameMusic.play();
}
    // --- STEP 2: LINK CHOICE TO LEVEL ---
    int level_number = choice; // This uses the 1 or 2 from your menu
    
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

       Font gameFont;
    if (!gameFont.loadFromFile("Data/Steelar-j9Vnj.otf")) {

    }
    bool showing_transition_text = false;
int transition_timer = 0;
const int transition_duration = 120; // 2 seconds at 60fps
string display_message = "";
bool is_paused = false;
    Text scoreText;
    scoreText.setFont(gameFont);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(20, 0);

    Text highScoreText;
    highScoreText.setFont(gameFont);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(Color::Yellow);
    highScoreText.setPosition(350, 10);

    Text livesText;
    livesText.setFont(gameFont);
    livesText.setCharacterSize(24);
    livesText.setFillColor(Color::Red);
    livesText.setPosition(700, 10);

    Text comboText;
    comboText.setFont(gameFont);
    comboText.setCharacterSize(20);
    comboText.setFillColor(Color::Cyan);
    comboText.setPosition(950, 10);
bool is_game_over = false;
       int total_score = 0;
    int high_score = 0;
    int current_lives = 3;
    int current_combo = 0;
    int combo_timer = 0;
    const int combo_timeout = 180;
    int level_finish_time = 0;

float slide_velocity = 0.0f;
const float slide_acceleration = 0.3f;
const float max_slide_speed = 6.0f;

	const int cell_size = 64;
	const int height = 14;
	const int width = 18;
	char** lvl;

int last_capture_frame = 0;
int frame_counter = 0;
float goal_x = 1000;
float goal_y = 200;
int goal_width = 64;
int goal_height = 64;
bool level_cleared = false;

	Texture bgTex;
	Sprite bgSprite;
	Texture blockTexture;
	Sprite blockSprite;

	Texture bagpacktexture;
	Sprite bagpacksprite;

	bgTex.loadFromFile("Data/ca.png");
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0,0);

	blockTexture.loadFromFile("Data/blockgg.png");
	blockSprite.setTexture(blockTexture);

	Music lvlMusic;

	lvlMusic.openFromFile("Data/mus.ogg");
	lvlMusic.setVolume(20);
	lvlMusic.play();
	lvlMusic.setLoop(true);

Texture potTexture;
Sprite potSprite;
float pot_x, pot_y;
int pot_health = 3;
bool pot_active = true;
bool pot_destroyed = false;
int pot_spawn_timer = 180;
int pot_spawn_interval = 180;
int pot_width = 64;
int pot_height = 64;

potTexture.loadFromFile("Data/pot.png");
potSprite.setTexture(potTexture);
potSprite.setScale(3,3);

int pot_platform_row = 3;
int pot_platform_col = 7;
pot_x = pot_platform_col * cell_size;
pot_y = (pot_platform_row * cell_size)-(64*3)+20;

potSprite.setPosition(pot_x, pot_y);

bool vacuum_active = false;
int vacuum_startup_timer = 0;
const int vacuum_startup_delay = 30;
int vacuum_range = 250;
int vacuum_direction = 0;
int max_bag_capacity = 3;
int enemies_in_bag = 0;
float vacuum_offset_x = 0;
float vacuum_offset_y = 0;
const float vacuum_move_speed = 5.0f;
int bagged_enemies[3] = {-1, -1, -1};
char bagged_enemy_types[3];
bool enemy_active[60];
bool enemy_stunned[60];
int enemy_stun_frames[60];
bool enemy_pulling[60];
int enemy_pull_frames[60];

Texture playerVacuumTexRight;
Texture playerVacuumTexLeft;
Sprite VacuumSprite;

playerVacuumTexLeft.loadFromFile("Data/vacuum_full.png");
playerVacuumTexRight.loadFromFile("Data/rvacuum_full.png");
int vacuum_frame = 0;
int vacuum_timer = 0;

const int VACUUM_SPEED = 8;

for(int pos = 0; pos < 60; pos++) {
    enemy_active[pos] = false;
    enemy_stunned[pos] = false;
    enemy_stun_frames[pos] = 0;
    enemy_pulling[pos] = false;
    enemy_pull_frames[pos] = 0;
}
int vacuum_cooldown_frames = 0;
int shoot_cooldown_frames = 0;

const int max_bullets = 10;
float bullet_x[10];
float bullet_y[10];
float bullet_speed_x[10];
float bullet_speed_y[10];
bool bullet_active[10];
char bullet_type[10];

	int PlayerHeight = 102;
	int PlayerWidth = 96;

CircleShape bullet_shapes[10];
for (int pos = 0; pos < max_bullets; pos++) {
    bullet_active[pos] = false;
}

	
float player_x = 64.0f; 
float player_y = (12 * 64) - PlayerHeight;

	bool player_is_dead = false;
bool is_invincible = false;
int invincibility_timer = 0;
const int invincibility_duration = 180; 
int death_frame_count = 0;
int respawn_delay = 60;

	float speed = 5;

	const float jumpStrength = -15;
	const float gravity = 1;

	bool isJumping = false;

	bool up_collide = false;
	bool left_collide = false;
	bool right_collide = false;

    Sprite PlayerSprite;
    bool facingRight = true;

	bool onGround = false;

	float offset_x = 0;
	float offset_y = 0;
	float velocityY = 0;

	float terminal_Velocity = 15;


	bool up_button = false;

	char top_left = '\0';
	char top_right = '\0';
	char top_mid = '\0';

	char left_mid = '\0';
	char right_mid = '\0';

	char bottom_left = '\0';
	char bottom_right = '\0';
	char bottom_mid = '\0';

	char bottom_left_down = '\0';
	char bottom_right_down = '\0';
	char bottom_mid_down = '\0';

	char top_right_up = '\0';
	char top_mid_up = '\0';
	char top_left_up = '\0';

    Texture slopeRightTexture;
Texture slopeLeftTexture;
Sprite slopeRightSprite;
Sprite slopeLeftSprite;

slopeRightTexture.loadFromFile("Data/rsbrick.png");
slopeLeftTexture.loadFromFile("Data/sbrick.png");

slopeRightSprite.setTexture(slopeRightTexture);
slopeLeftSprite.setTexture(slopeLeftTexture);

	const int max_ghosts = 20;
    int num_ghosts = 0;
    float ghost_x[20];
    float ghost_y[20];
    float ghost_speed[20];
    bool ghost_movingRight[20];
    Sprite ghost_sprites[20];
    Texture ghostTexture;
    ghostTexture.loadFromFile("Data/ghost.png");

const int max_skeletons = 20;
int num_skeletons = 0;
float skeleton_x[20];
float skeleton_y[20];
float skeleton_speed[20];
bool skeleton_movingRight[20];
float skeleton_velocityY[20];
bool skeleton_onGround[20];
int skeleton_wait_frames[20];
bool skeleton_running[20];
Sprite skeleton_sprites[20];
Texture skeletonTextureRight;
Texture skeletonTextureLeft;
skeletonTextureRight.loadFromFile("Data/rskeleton.png");
skeletonTextureLeft.loadFromFile("Data/skeleton.png");

const int max_chelnovs = 20;
int num_chelnov= 0;
float chelnov_x[20];
float chelnov_y[20];
float chelnov_speed[20];
bool chelnov_movingRight[20];
float chelnov_velocityY[20];
bool chelnov_onGround[20];
int chelnov_wait_frames[20];
bool chelnov_running[20];
int chelnov_shoot_timer[20];
bool chelnov_is_shooting[20];
int chelnov_shoot_phase_timer[20];
Sprite chelnov_sprites[20];

Texture chelnovTextureRight;
Texture chelnovTextureLeft;
chelnovTextureRight.loadFromFile("Data/RChelnovWalk.png");
chelnovTextureLeft.loadFromFile("Data/ChelnovWalk.png");

const int max_invisible = 20;
int num_invisible = 0;
float invisible_x[20];
float invisible_y[20];
float invisible_speed[20];
bool invisible_movingRight[20];
int invisible_wait_frames[20];
bool invisible_teleporting[20];
bool invisible_visible[20];
int invisible_visibility_timer[20];
int invisible_anim_frame[20];
int invisible_anim_timer[20];
float invisible_target_y[20];
Sprite invisible_sprites[20];

Texture invisibleTextureRight;
Texture invisibleTextureLeft;
Texture invisibleStopTextures[3];
Texture invisibleFadeTextures[4];
enum GameState { MENU, PLAYING };
    GameState currentGS = MENU;

float vac_part_x[15], vac_part_y[25];
bool vac_part_active[15] = { false };
float vac_part_speed[15];
for (int pos = 0; pos < max_invisible; pos++) {
    invisible_target_y[pos] = 0;
    invisible_anim_frame[pos] = 0;
    invisible_anim_timer[pos] = 0;
}

const int max_projectiles = 50;
float projectile_x[50];
float projectile_y[50];
float projectile_speed_x[50];
float projectile_speed_y[50];
bool projectile_active[50];
Sprite projectile_sprites[50];

Texture projectileTexture;
projectileTexture.loadFromFile("Data/fireball.png");

for (int pos = 0; pos < max_projectiles; pos++) {
    projectile_active[pos] = false;
    projectile_sprites[pos].setTexture(projectileTexture);
    projectile_sprites[pos].setScale(2, 2);
}
       PlayerSprite.setTexture(PlayerTextureLeft);
    if(selectedplayernum == 1){
    PlayerTextureLeft.loadFromFile("Data/PGwalk.png");
    PlayerTextureRight.loadFromFile("Data/RPGwalk.png");
    }else{
    PlayerTextureLeft.loadFromFile("Data/PYwalk.png");
    PlayerTextureRight.loadFromFile("Data/RPYwalk.png");
    }

    PlayerSprite.setTexture(PlayerTextureRight);
    PlayerSprite.setTextureRect(IntRect(0,0,31,40));

	PlayerSprite.setScale(3,3);
    bagpacktexture.loadFromFile("Data/bpr.png");
	bagpacksprite.setTexture(bagpacktexture);
    bagpacksprite.setScale(3, 3);

	lvl = new char* [height];
	for (int pos = 0; pos < height; pos += 1)
	{
		lvl[pos] = new char[width];
	}

bool level_complete = false;
int level_complete_timer = 0;
const int level_complete_delay = 180;

// This now uses the 'level_number' we got from the menu!
load_level(lvl, height, width, level_number);

// Adjust enemy counts based on the selection
if (level_number == 1) {
    num_ghosts = 4;
    num_skeletons = 2;
} else if (level_number == 2) {
    num_ghosts = 6;
    num_skeletons = 4;
}
spawn_ghosts(ghost_x, ghost_y, ghost_speed, ghost_movingRight, ghost_sprites,
             num_ghosts, lvl, height, width, cell_size, ghostTexture, PlayerHeight);

spawn_skeletons(skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight,
                skeleton_velocityY, skeleton_onGround, skeleton_wait_frames,
                skeleton_running, skeleton_sprites, num_skeletons, lvl, height,
                width, cell_size, skeletonTextureRight, skeletonTextureLeft, PlayerHeight);
spawn_chelnovs(chelnov_x, chelnov_y, chelnov_speed, chelnov_movingRight,
               chelnov_velocityY, chelnov_onGround, chelnov_wait_frames, chelnov_running,
               chelnov_shoot_timer, chelnov_is_shooting, chelnov_shoot_phase_timer,
               chelnov_sprites, num_chelnov, lvl, height, width, cell_size,
               chelnovTextureRight, chelnovTextureLeft, PlayerHeight);
spawn_invisible_men(invisible_x, invisible_y, invisible_speed, invisible_movingRight,
                   invisible_wait_frames, invisible_teleporting, invisible_visible,
                   invisible_visibility_timer, invisible_anim_frame, invisible_anim_timer,
                   invisible_sprites, num_invisible, lvl, height, width, cell_size,
                   invisibleTextureRight, invisibleTextureLeft, PlayerHeight);

int ghost_start_index = 0;
int skeleton_start_index = 20;
int chelnov_start_index = 40;
int invisible_start_index = 50;

for (int pos = 0; pos < num_ghosts; pos++) {
    enemy_active[ghost_start_index + pos] = true;
}
for (int pos = 0; pos < num_skeletons; pos++) {
    enemy_active[skeleton_start_index + pos] = true;
}
for (int pos = 0; pos < num_chelnov; pos++) {
    enemy_active[chelnov_start_index + pos] = true;
}

for (int pos = 0; pos < max_invisible; pos++) {
    invisible_target_y[pos] = 0;
}

	Event ev;
Text transitionText;
transitionText.setFont(gameFont);
transitionText.setCharacterSize(60);
transitionText.setFillColor(Color::Yellow);
transitionText.setOutlineColor(Color::Black);
transitionText.setOutlineThickness(3);
// --- 1. THE MASTER LOOP (Wraps everything: Menu -> Selection -> Game) ---
// --- 1. THE MASTER LOOP (Menu -> Selection -> Game) ---
while (window.isOpen()) 
{
    // --- 2. SHOW MAIN MENU ---
    int choice = showmenu(window); 

    if (choice == 0 || !window.isOpen()) break; 
    
    if (choice == 3) {
        // show_instructions(window);
        continue; 
    }

    // --- 3. CHARACTER SELECTION ---
    selectedplayernum = selectedplayer(window, PlayerTextureRight, PlayerTextureLeft);

    // *** FIX FOR FIRST-TIME MENU SKIP (The Flush) ***
    Event flush;
    while (window.pollEvent(flush)); 

    // --- 4. RESET ALL SESSION VARIABLES ---
    bool playingLevel = true; 
    is_paused = false;
    is_game_over = false;
    current_lives = 3;
    total_score = 0;
    player_is_dead = false;
    velocityY = 0;
    onGround = false;
    is_invincible = true; 
    invincibility_timer = 180; 
    showing_transition_text = false;
    enemies_in_bag = 0;
    for (int i = 0; i < 3; i++) bagged_enemies[i] = -1;

    // Set Level based on Menu Choice
    if (choice == 2) {
        level_number = 2;
        player_x = 64.0f; 
        player_y = (12.0f * cell_size) - PlayerHeight;
    } else {
        level_number = 1;
        player_x = 500.0f; 
        player_y = 150.0f;
    }

    load_level(lvl, height, width, level_number);

    // Spawning Logic
    for (int i = 0; i < 60; i++) enemy_active[i] = false;
    if (level_number == 1) { num_ghosts = 4; num_skeletons = 2; } 
    else { num_ghosts = 6; num_skeletons = 4; }
    
    spawn_ghosts(ghost_x, ghost_y, ghost_speed, ghost_movingRight, ghost_sprites, num_ghosts, lvl, height, width, cell_size, ghostTexture, PlayerHeight);
    spawn_skeletons(skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight, skeleton_velocityY, skeleton_onGround, skeleton_wait_frames, skeleton_running, skeleton_sprites, num_skeletons, lvl, height, width, cell_size, skeletonTextureRight, skeletonTextureLeft, PlayerHeight);
    
    for (int i = 0; i < num_ghosts; i++) enemy_active[ghost_start_index + i] = true;
    for (int i = 0; i < num_skeletons; i++) enemy_active[skeleton_start_index + i] = true;

    // --- 5. THE GAMEPLAY LOOP ---
    while (window.isOpen() && playingLevel)
    {
        bool returnToMenu = false;
        Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed) { window.close(); playingLevel = false; }
            if (ev.type == Event::KeyPressed) {
                if (ev.key.code == Keyboard::Escape || ev.key.code == Keyboard::P) {
                    if (!is_game_over) is_paused = !is_paused; 
                }
            }
            if (ev.type == Event::KeyPressed && is_game_over) {
                if (ev.key.code == Keyboard::R) {
                    // Quick Reset to Level 1
                    is_game_over = false; current_lives = 3; total_score = 0; level_number = 1;
                    load_level(lvl, height, width, level_number);
                    player_x = 500; player_y = 150; player_is_dead = false;
                    is_invincible = true; invincibility_timer = 180; 
                    for (int i = 0; i < 60; i++) enemy_active[i] = false;
                    num_ghosts = 4; num_skeletons = 2;
                    spawn_ghosts(ghost_x, ghost_y, ghost_speed, ghost_movingRight, ghost_sprites, num_ghosts, lvl, height, width, cell_size, ghostTexture, PlayerHeight);
                    spawn_skeletons(skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight, skeleton_velocityY, skeleton_onGround, skeleton_wait_frames, skeleton_running, skeleton_sprites, num_skeletons, lvl, height, width, cell_size, skeletonTextureRight, skeletonTextureLeft, PlayerHeight);
                    for (int i = 0; i < num_ghosts; i++) enemy_active[ghost_start_index + i] = true;
                    for (int i = 0; i < num_skeletons; i++) enemy_active[skeleton_start_index + i] = true;
                }
            }
        }

        if (!is_paused && !is_game_over) 
        {
            // Logic Updates
            if (!player_is_dead) {
                player_gravity(lvl, offset_y, velocityY, onGround, gravity, terminal_Velocity, player_x, player_y, cell_size, PlayerHeight, PlayerWidth, screen_y, height, width);
                player_movement(lvl, player_x, player_y, velocityY, onGround, cell_size, PlayerWidth, PlayerHeight, speed, jumpStrength, PlayerSprite, PlayerTextureRight, PlayerTextureLeft, facingRight, screen_x, screen_y, height, width, selectedplayernum);
            }

            // Vacuum Animation & Pulling
            bool vacuum_was_active = vacuum_active;
            bool vacuum_was_pulling_before = (vacuum_was_active && vacuum_startup_timer >= vacuum_startup_delay);
            if (!player_is_dead) {
                if (Keyboard::isKeyPressed(Keyboard::A)) {
                    if (facingRight) { if (!vacuum_active || vacuum_direction != 3) { vacuum_active = true; vacuum_startup_timer = 0; vacuum_direction = 3; } else vacuum_startup_timer++; }
                    else { if (!vacuum_active || vacuum_direction != 0) { vacuum_active = true; vacuum_startup_timer = 0; vacuum_direction = 0; } else vacuum_startup_timer++; }
                }
                else if (Keyboard::isKeyPressed(Keyboard::D)) {
                    if (facingRight) { if (!vacuum_active || vacuum_direction != 0) { vacuum_active = true; vacuum_startup_timer = 0; vacuum_direction = 0; } else vacuum_startup_timer++; }
                    else { if (!vacuum_active || vacuum_direction != 3) { vacuum_active = true; vacuum_startup_timer = 0; vacuum_direction = 3; } else vacuum_startup_timer++; }
                }
                else if (Keyboard::isKeyPressed(Keyboard::W)) { if (!vacuum_active || vacuum_direction != 1) { vacuum_active = true; vacuum_startup_timer = 0; vacuum_direction = 1; } else vacuum_startup_timer++; }
                else if (Keyboard::isKeyPressed(Keyboard::S)) { if (!vacuum_active || vacuum_direction != 2) { vacuum_active = true; vacuum_startup_timer = 0; vacuum_direction = 2; } else vacuum_startup_timer++; }
                else { vacuum_active = false; vacuum_startup_timer = 0; }
            }

            bool vacuum_is_pulling_now = (vacuum_active && vacuum_startup_timer >= vacuum_startup_delay);
            update_vacuum_animation(vacuum_active, vacuum_was_pulling_before, vacuum_frame, vacuum_timer, VacuumSprite, playerVacuumTexRight, playerVacuumTexLeft, facingRight, player_x, player_y, PlayerWidth, PlayerHeight, enemy_pulling, enemy_stunned, enemy_stun_frames, vacuum_offset_x, vacuum_offset_y, num_ghosts, ghost_start_index, num_skeletons, skeleton_start_index, skeleton_onGround, num_chelnov, chelnov_start_index, chelnov_onGround, num_invisible, invisible_start_index);
            if (!vacuum_active) update_stun_frames(enemy_stunned, enemy_stun_frames);
            vacuum_pull_enemies(vacuum_is_pulling_now, enemies_in_bag, max_bag_capacity, player_x, player_y, PlayerWidth, PlayerHeight, facingRight, vacuum_range, cell_size, vacuum_direction, ghost_x, ghost_y, num_ghosts, ghost_start_index, skeleton_x, skeleton_y, num_skeletons, skeleton_start_index, chelnov_x, chelnov_y, num_chelnov, chelnov_start_index, enemy_active, enemy_stunned, enemy_pulling, enemy_stun_frames, enemy_pull_frames, bagged_enemies, bagged_enemy_types, vacuum_offset_x, vacuum_offset_y, invisible_x, invisible_y, num_invisible, invisible_start_index, invisible_teleporting, total_score, high_score, current_combo, combo_timer);

            // Combat & Enemies
            if (shoot_cooldown_frames > 0) shoot_cooldown_frames--;
            if (Keyboard::isKeyPressed(Keyboard::C)) { shoot_bullet(enemies_in_bag, shoot_cooldown_frames, bullet_x, bullet_y, bullet_speed_x, bullet_speed_y, bullet_active, bullet_type, bullet_shapes, max_bullets, player_x, player_y, PlayerWidth, facingRight, bagged_enemies, bagged_enemy_types, ghostTexture, skeletonTextureRight, chelnovTextureRight); }
            update_bullets(bullet_x, bullet_y, bullet_speed_x, bullet_speed_y, bullet_active, bullet_shapes, max_bullets, lvl, height, width, cell_size, screen_x, screen_y, ghost_x, ghost_y, num_ghosts, ghost_start_index, skeleton_x, skeleton_y, num_skeletons, skeleton_start_index, chelnov_x, chelnov_y, num_chelnov, chelnov_start_index, enemy_active, pot_active, pot_destroyed, pot_height, pot_width, pot_health, pot_x, pot_y, total_score, high_score, current_combo, combo_timer);
            update_ghosts(ghost_x, ghost_y, ghost_speed, ghost_movingRight, ghost_sprites, num_ghosts, lvl, height, width, cell_size, enemy_active, ghost_start_index, enemy_stunned, enemy_pulling);
            update_skeletons(skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight, skeleton_velocityY, skeleton_onGround, skeleton_wait_frames, skeleton_running, skeleton_sprites, num_skeletons, lvl, height, width, cell_size, screen_x, screen_y, skeletonTextureRight, skeletonTextureLeft, enemy_active, skeleton_start_index, enemy_stunned, enemy_pulling);
            update_chelnovs(chelnov_x, chelnov_y, chelnov_speed, chelnov_movingRight, chelnov_velocityY, chelnov_onGround, chelnov_wait_frames, chelnov_running, chelnov_shoot_timer, chelnov_is_shooting, chelnov_shoot_phase_timer, chelnov_sprites, num_chelnov, lvl, height, width, cell_size, screen_x, screen_y, chelnovTextureRight, chelnovTextureLeft, enemy_active, chelnov_start_index, enemy_stunned, enemy_pulling, player_x, player_y, projectile_x, projectile_y, projectile_speed_x, projectile_speed_y, projectile_active, max_projectiles);
            update_invisible_men(invisible_x, invisible_y, invisible_speed, invisible_movingRight, invisible_wait_frames, invisible_teleporting, invisible_visible, invisible_visibility_timer, invisible_anim_frame, invisible_anim_timer, invisible_target_y, invisible_sprites, num_invisible, lvl, height, width, cell_size, screen_x, screen_y, enemy_active, invisible_start_index, enemy_stunned, enemy_pulling, invisibleTextureRight, invisibleTextureLeft, invisibleStopTextures, invisibleFadeTextures);
            update_projectiles(projectile_x, projectile_y, projectile_speed_x, projectile_speed_y, projectile_active, max_projectiles, screen_x, screen_y, lvl, height, width, cell_size, player_x, player_y, PlayerWidth, PlayerHeight, player_is_dead);

            // Transitions & Collision
            if (current_combo > 0) { combo_timer++; if (combo_timer >= combo_timeout) { current_combo = 0; combo_timer = 0; } }
            bool any_enemies_left = false;
            for (int i = 0; i < 60; i++) { if (enemy_active[i]) { any_enemies_left = true; break; } }
            if (!any_enemies_left && level_number == 1 && !showing_transition_text) { showing_transition_text = true; transition_timer = transition_duration; display_message = "LEVEL 1 CLEARED!"; }

            if (showing_transition_text) {
                transition_timer--;
                if (transition_timer <= 0) {
                    if (display_message == "LEVEL 1 CLEARED!") {
                        level_number = 2; load_level(lvl, height, width, level_number);
                        player_x = 64.0f; player_y = (12.0f * cell_size) - PlayerHeight; velocityY = 0;
                        is_invincible = true; invincibility_timer = 180;
                        display_message = "LEVEL 2 START"; transition_timer = transition_duration;
                        num_ghosts = 6; num_skeletons = 4;
                        spawn_ghosts(ghost_x, ghost_y, ghost_speed, ghost_movingRight, ghost_sprites, num_ghosts, lvl, height, width, cell_size, ghostTexture, PlayerHeight);
                        spawn_skeletons(skeleton_x, skeleton_y, skeleton_speed, skeleton_movingRight, skeleton_velocityY, skeleton_onGround, skeleton_wait_frames, skeleton_running, skeleton_sprites, num_skeletons, lvl, height, width, cell_size, skeletonTextureRight, skeletonTextureLeft, PlayerHeight);
                        for (int i = 0; i < 60; i++) enemy_active[i] = false;
                        for (int i = 0; i < num_ghosts; i++) enemy_active[ghost_start_index + i] = true;
                        for (int i = 0; i < num_skeletons; i++) enemy_active[skeleton_start_index + i] = true;
                    } else showing_transition_text = false;
                }
            }

            if (!player_is_dead && !is_invincible) {
                if (check_player_ghost_collision(player_x, player_y, PlayerWidth, PlayerHeight, ghost_x, ghost_y, num_ghosts, enemy_active, ghost_start_index, enemy_stunned, enemy_pulling) ||
                    check_player_skeleton_collision(player_x, player_y, PlayerWidth, PlayerHeight, skeleton_x, skeleton_y, num_skeletons, enemy_active, skeleton_start_index, enemy_stunned, enemy_pulling) ||
                    check_player_chelnov_collision(player_x, player_y, PlayerWidth, PlayerHeight, chelnov_x, chelnov_y, num_chelnov, enemy_active, chelnov_start_index, enemy_stunned, enemy_pulling, chelnov_is_shooting) ||
                    check_player_invisible_collision(player_x, player_y, PlayerWidth, PlayerHeight, invisible_x, invisible_y, num_invisible, enemy_active, invisible_start_index, enemy_stunned, enemy_pulling, invisible_teleporting)) {
                    player_is_dead = true; death_frame_count = 0; current_lives--;
                    total_score = (total_score - 50 < 0) ? 0 : total_score - 50;
                    is_invincible = true; invincibility_timer = 180;
                }
            }
            else if (player_is_dead) {
                player_x = -2000.0f; player_y = -2000.0f; death_frame_count++;
                if (current_lives <= 0) is_game_over = true;
                if (!is_game_over && death_frame_count >= respawn_delay) {
                    player_x = (float)(1 * cell_size); player_y = (float)((height - 2) * cell_size) - PlayerHeight;
                    velocityY = 0; onGround = false; player_is_dead = false; death_frame_count = 0; vacuum_active = false;
                }
            }
if (vacuum_active && vacuum_startup_timer >= vacuum_startup_delay) {
    // 1. Define the Nozzle Position (The target where air goes in)
    float nozzle_x = player_x + (facingRight ? PlayerWidth + 5 : -5);
    float nozzle_y = player_y + (PlayerHeight / 2) - 5; 

    for (int i = 0; i < 15; i++) {
        if (!vac_part_active[i]) {
            vac_part_active[i] = true;
            vac_part_speed[i] = 6.0f + (rand() % 4);
            
            // 2. Spawn point: Start far away and pull IN
            float range = 180.0f; 
            vac_part_x[i] = nozzle_x + (facingRight ? range : -range);
            // Add some vertical randomness so it looks like a funnel
            vac_part_y[i] = nozzle_y + (rand() % 80 - 40); 
        } else {
            // 3. Move logic: Move directly toward the nozzle
            if (facingRight) {
                vac_part_x[i] -= vac_part_speed[i]; // Move Left toward player
                if (vac_part_x[i] <= nozzle_x) vac_part_active[i] = false;
            } else {
                vac_part_x[i] += vac_part_speed[i]; // Move Right toward player
                if (vac_part_x[i] >= nozzle_x) vac_part_active[i] = false;
            }

            // Slowly pull Y toward the nozzle center (funnel effect)
            if (vac_part_y[i] < nozzle_y) vac_part_y[i] += 1.5f;
            else vac_part_y[i] -= 1.5f;
        }
    }
} else {
    for (int i = 0; i < 15; i++) vac_part_active[i] = false;
}
        }

        // --- 6. DRAWING ---
        window.clear();
        display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, slopeRightTexture, slopeRightSprite, slopeLeftTexture, slopeLeftSprite, height, width, cell_size);

        for (int i = 0; i < num_ghosts; i++) if (enemy_active[ghost_start_index + i]) window.draw(ghost_sprites[i]);
        for (int i = 0; i < num_skeletons; i++) if (enemy_active[skeleton_start_index + i]) window.draw(skeleton_sprites[i]);
        for (int i = 0; i < num_chelnov; i++) if (enemy_active[chelnov_start_index + i]) window.draw(chelnov_sprites[i]);
        for (int i = 0; i < num_invisible; i++) if (enemy_active[invisible_start_index + i] && invisible_visible[i]) window.draw(invisible_sprites[i]);
        for (int i = 0; i < max_projectiles; i++) if (projectile_active[i]) { projectile_sprites[i].setPosition(projectile_x[i], projectile_y[i]); window.draw(projectile_sprites[i]); }

        if (!player_is_dead) {
            // Player Visuals
            float final_scale_x = 3.0f, final_scale_y = 3.0f;
            if (velocityY < -1.0f) { final_scale_x = 3.0f * 0.85f; final_scale_y = 3.0f * 1.15f; }
            else if (velocityY > 2.0f && !onGround) { final_scale_x = 3.0f * 1.15f; final_scale_y = 3.0f * 0.85f; }
            
            if (facingRight) PlayerSprite.setTexture(PlayerTextureRight); else PlayerSprite.setTexture(PlayerTextureLeft);
            PlayerSprite.setScale(final_scale_x, final_scale_y);
            PlayerSprite.setPosition(player_x, player_y - 10);

            // Backpack Positioning & Scaling
            if (facingRight) { 
                bagpacksprite.setScale(3.0f, 3.0f); 
                bagpacksprite.setPosition(player_x-10, player_y + 10); 
            } else { 
                bagpacksprite.setScale(-3.0f, 3.0f); 
                bagpacksprite.setPosition(player_x + PlayerWidth + 12, player_y + 10); 
            }

            // Invincibility Transparency Fix
            if (is_invincible) {
                if (((invincibility_timer) / 5) % 2 == 0) PlayerSprite.setColor(Color(255, 255, 255, 80));
                else PlayerSprite.setColor(Color(255, 255, 255, 255));
                invincibility_timer--; 
                if (invincibility_timer <= 0) is_invincible = false;
            } else {
                PlayerSprite.setColor(Color(255, 255, 255, 255));
            }

            window.draw(bagpacksprite);
            if (vacuum_active) window.draw(VacuumSprite);
            window.draw(PlayerSprite);
        }

        draw_bullets(window, bullet_active, bullet_shapes, bullet_x, bullet_y, max_bullets);
        
        // --- UI DRAWING (Restored) ---
        scoreText.setString("SCORE: " + to_string(total_score));
        highScoreText.setString("HIGH: " + to_string(high_score));
        livesText.setString("LIVES: " + to_string(current_lives));
        window.draw(scoreText); 
        window.draw(highScoreText); 
        window.draw(livesText);
        if (current_combo >= 1) window.draw(comboText);

        if (showing_transition_text){
            transitionText.setString(display_message);
            FloatRect tr = transitionText.getLocalBounds();
            transitionText.setOrigin(tr.left + tr.width / 2.0f, tr.top + tr.height / 2.0f);
            transitionText.setPosition(screen_x / 2.0f, screen_y / 2.0f);
            window.draw(transitionText);
        }

        if (is_paused) {
            draw_pause_menu(window, gameFont, is_paused, returnToMenu);
            if (returnToMenu) playingLevel = false; 
        }

        if (is_game_over) {
            RectangleShape overlay(Vector2f(screen_x, screen_y));
            overlay.setFillColor(Color(0, 0, 0, 150));
            window.draw(overlay);
            Text gameOverText("GAME OVER\nPRESS 'R' TO RESTART", gameFont, 50);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setOutlineColor(Color::Black); gameOverText.setOutlineThickness(5);
            FloatRect gr = gameOverText.getLocalBounds();
            gameOverText.setOrigin(gr.left + gr.width / 2.0f, gr.top + gr.height / 2.0f);
            gameOverText.setPosition(screen_x / 2.0f, screen_y / 2.0f);
            window.draw(gameOverText);
        }

        if (vacuum_active) {
    for (int i = 0; i < 15; i++) {
        if (vac_part_active[i]) {
            RectangleShape streak(Vector2f(12.0f, 2.5f)); // Thin wind line
            streak.setPosition(vac_part_x[i], vac_part_y[i]);
            streak.setFillColor(Color(255, 255, 255, 150)); // Semi-transparent white
            window.draw(streak);
        }
    }
}

        window.display();
    } // End Gameplay
} // End Master
    lvlMusic.stop();
    for (int pos = 0; pos < height; pos++)
    {
        delete[] lvl[pos];
    }
    delete[] lvl;

	return 0;
}
