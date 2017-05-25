#include "../../inc/models/world.hpp"
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))


int World::SEA_LEVEL=0;

BufferedLogger worldLogger = BufferedLogger("world");

World::World() {
    const int radius = 200;
    const int c_x=500, c_y=500;

    randomGenerator.init(0, width, 1337);

    grass = std::vector<std::vector<float>>(width, std::vector<float>(height, 0));

    worldLogger.log("world created", "SUCC");    
}

bool World::isWinter() {
    return winter;
}

void World::generate() {

    int size = max(width, height);
    WorldGenerator generator(width, 1337);
    Array2DKeeper<int> hmap(width, height);

    generator.generate(hmap.getArray2D());


    int** array2d = hmap.getArray2D();

    int cnt = 0;
    for(int x=0;x<width;x++)
        for(int y=0;y<height;y++) {
            h_map[x][y] = array2d[x][y];
            if (h_map[x][y] >= 0) cnt++;
        }

    if ((cnt < size * size * MIN_LAND) || (cnt > size * size * MAX_LAND)) {
        generate();
        worldLogger.log("regenerating world", "INFO");
    }

}

pair<int, int> World::getRandomLand() {
    int x, y;

    do {
        x = randomGenerator.next();
        y = randomGenerator.next();
    } while(!isLandAt(x, y));

    return pair<int, int>(x, y);
}

bool World::isLandAt(int x, int y) {
    if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
        return h_map[x][y] > SEA_LEVEL;
    }
    else {
        return false;
    }
}

bool World::isLandAt(pair<float, float> coords) {
    int x = coords.a;
    int y = coords.b;

    if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
        return h_map[x][y] > SEA_LEVEL;
    }
    else {
        return false;
    }
}

int World::getGrassAt(int x, int y) {
    return grass[x][y];
}

void World::spawnGrass() {
    worldLogger.log("spawning grass", "INFO");
    for (int x=0;x<width;x++) for(int y=0;y<height;y++) {
        grass[x][y] = (isLandAt(x, y) ? (0.5 * GRASS_MAX) : 0);
    }
    worldLogger.log("grass spawned", "SUCC");
}


void World::tick() {
    // grow grass
    for (int x=0;x<width;x++) for(int y=0;y<height;y++) {
        if (grass[x][y] < GRASS_MAX - grass[x][y] * GRASS_GROWTH_RATE) {
            grass[x][y] += GRASS_GROWTH_RATE;
        }
    }

    time++;

    if ((time == CATACLYSM_TIME) && (! winter)) {
        time = 0;
        winter = true;
        std::cout << "winter has begun\n";
    } else if ((time == CATACLYSM_DURATION) && (winter)) {
        time = 0;
        winter = false;
        std::cout << "winter has ended\n";
    }
}


int World::getTime() {
    return time;
}

int World::getTemperature() {
    if (winter) {
        return baseTemperature;
    } else {
        return baseTemperature - 40;
    }
}

int World::cutGrass(int x, int y, int r, int max, bool forReal) { 
    int d;
    int cutting[2 * r + 1][2 * r + 1];

    int sum = 0;
    for (int i=-r;i<r+1;i++) for(int j=-r;j<r+1;j++) {
        if ((x + i >= 0) && (x + i < width) && (y + j >=0) && (y + j < height)) {
            d = grass[x+i][y+j] * GRASS_CUTTING_COEF;
            cutting[r + i][r + j] = d;
            sum += d;
        }
    }

    //std::cout << "sum1 " << sum << " max " << max <<"\n";

    if (sum > max) {
        for (int i=0;i<2*r+1;i++) for(int j=0;j<2*r+1;j++) {
            cutting[i][j] *= max;
            cutting[i][j] /= sum;
        }
    }

    sum = 0;

    for (int i=-r;i<r+1;i++) for(int j=-r;j<r+1;j++) {
        if ((x + i >= 0) && (x + i < width) && (y + j >=0) && (y + j < height)) {
            d = cutting[r + i][r + j];
            if (forReal) grass[x+i][y+j] -= d;
            sum += d;
        }
    }

    //std::cout << "sum2 " << sum << "\n";

    return sum * GRASS_CUTTING_EFFICIENCY;
}