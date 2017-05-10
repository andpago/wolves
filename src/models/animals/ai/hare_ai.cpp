const int ALERT_DIST = 30;
const int SAMPLING_DIST = 50;
const int CLUSTER_DIST = 20;
const int MAX_CLUSTER_SIZE = 4;

pair<float, float> coords = getCoords();
const float food = world->cutGrass(coords.a, coords.b, 2, 10000, false);

pair<float, float> antiWolf(0, 0);
pair<float, float> antiWater(0, 0);
pair<float, float> antiHunger(0, 0);
pair<float, float> antiHare(0, 0);
pair<float, float> vec_rel, vec;

vector<pair<float, float>> samples = getCircleOfAngledVectors(16);

int clusterSize = 0;

for (auto v=samples.begin(); v!= samples.end(); v++) {
	vec_rel = (*v).scale(SAMPLING_DIST);
	vec = coords + vec_rel;
	if (!world->isLandAt(vec)) {
		antiWater = antiWater + - vec_rel;
	} else {
		vec = vec_rel.scale(5) + coords;
		antiHunger = antiHunger + vec_rel.scale(world->cutGrass(vec.a, vec.b, 2, 10000, false) - food);

		vec = vec_rel.scale(50) + coords;
		antiHunger = antiHunger + vec_rel.scale(world->cutGrass(vec.a, vec.b, 2, 10000, false) - food);

		vec = vec_rel.scale(300) + coords;
		antiHunger = antiHunger + vec_rel.scale(world->cutGrass(vec.a, vec.b, 2, 10000, false) - food);
	}
}

for (auto it=begin(); it != end(); it++) if (*it != this) {
	float d = dist(coords, (*it)->getCoords());

	if ((*it)->isHare()) {
		if (d != 0) {
			antiHare = antiHare + (-(*it)->getCoords() + coords).scale(CLUSTER_DIST * 1.0 / d * (1 - 1.0 * (*it)->getHunger() / maxHunger)); 
		} else {
			//antiHare = antiHare + vectorRandom.next();
		}

		if (d < CLUSTER_DIST) clusterSize++;
	} else {
		if (d < ALERT_DIST) return coords + - (*it) -> getCoords();
		else antiWolf = antiWolf + (-(*it)->getCoords() + coords).scale(1 / d);
	}
}

if (clusterSize > MAX_CLUSTER_SIZE) return antiHare;

// return pair<float, float>(0, 0);
return antiWolf.scale(0.1) + antiWater.scale(2) + antiHunger.scale(1) + antiHare.scale((MAX_BREEDING_HUNGER - hunger) * 0.5 / MAX_BREEDING_HUNGER) + getSpeedVector().scale(1);