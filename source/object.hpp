#pragma once

#include "transform.hpp"
#include "timer.hpp"
#include "graphics/sprite_animation.hpp"

#include <optional>
#include <vector>

class game_program;

enum class object_type : unsigned char {

	player,
	
	// big directions
	bush,
	tree,
	big_rock,
	tree_stump,
	log,
	
	// small decorations
	small_rocks,

	// ingredients
	big_red_mushroom,
	fat_blue_mushroom,

	no_ingredient,
	start_brewing,
	star_anise,
	st_johns_wort,
	spearmint,
	solomons_seal,
	thyme,
	valerian,
	wormwood,
	sage,
	patchouli,
	hyssop,
	majoram,
	horehound,

	// building
	house_exterior,
	house_interior,

	// tools
	brewing_stand,

	// treatments
	no_cure,
	bad_potion,
	bald_cure,
	barbarian_mix,
	colorful_potion,
	thick_mixture,
	youth_potion,
	eagle_potion,

	// characters
	balding_monk,
	afro_monk,

	fat_virgin,
	fit_chad,

	gray_hair_lumberjack,
	blonde_hair_lumberjack,

	thin_hair_lady,
	thick_hair_lady,

	saggy_tits_lady,
	big_booba_lady,

	cant_see_shit_granny,
	eagle_eyed_granny,

	fat_monk,
	fit_monk,

	gray_hair_monk,
	brown_hair_monk,

	weak_monk,
	strong_monk,

	poor_sight_monk,
	good_sight_monk,

	balding_man,
	afro_man,

	gray_hair_man,
	blonde_hair_man,

	weak_man,
	strong_man,

	poor_sight_man,
	good_sight_man,

	balding_lumberjack,
	afro_lumberjack,

	fat_lumberjack,
	fit_lumberjack,

	fat_lumberjack_2,
	fit_lumberjack_2,

	poor_sight_lumberjack,
	good_sight_lumberjack,

	fat_lady,
	healthy_lady,

	gray_hair_lady,
	blonde_hair_lady,

	old_lady,
	young_lady,

	poor_sight_lady,
	good_sight_lady,

	balding_lady,
	hairful_lady,

	fat_lady_2,
	healthy_lady_2,

	gray_hair_lady_2,
	brown_hair_lady,

	poor_sight_lady_2,
	good_sight_lady_2,

	thin_hair_lady_2,
	thick_hair_lady_2,

	fat_lady_3,
	healthy_lady_3,

	gray_hair_lady_3,
	black_hair_lady,

	poor_sight_lady_3,
	good_sight_lady_3

};

enum class object_direction : unsigned char { left, right };

class game_object {
public:

	nfwk::transform2 transform;
	nfwk::transform2 collision_transform;
	nfwk::vector2f tex_coords;
	object_direction direction{ object_direction::right };
	object_type type;
	bool interactive{ false };
	bool customer{ false };
	nfwk::transform2 collision;
	float y_off{ 0.0f };
	bool y_off_dir{ false };

	game_object(object_type type);

	virtual void set_type(object_type type);

	virtual void update(game_program& game);

	static std::string get_name(object_type type) {
		switch (type) {
			using enum object_type;
			
			// potions
		case no_cure: return "No cure";
		case bad_potion: return "Potion";
		case bald_cure: return "Bald Cure";
		case barbarian_mix: return"Barbarian Mix";
		case colorful_potion: return "Colorful Potion";
		case thick_mixture: return "Thick Mixture";
		case youth_potion: return "Youth Potion";
		case eagle_potion: return "Eagle's Potion";

			// ingredients
		case start_brewing: return "Brew";
		case no_ingredient: return "Cancel";
		case star_anise: return "Star Anise";
		case st_johns_wort: return "St. John's Wort";
		case spearmint: return "Spearmint";
		case solomons_seal: return "Solomon's Seal";
		case thyme: return "Thyme";
		case valerian: return "Valerian";
		case wormwood: return "Wormwood";
		case sage: return "Sage";
		case patchouli: return "Patchouli";
		case hyssop: return "Hyssop";
		case majoram: return "Majoram";
		case horehound: return "Horehound";

		default: return "";
		}
	}

	static bool is_treatment(object_type type) {
		switch (type) {
			using enum object_type;
		case bad_potion:
		case bald_cure:
		case barbarian_mix:
		case colorful_potion:
		case thick_mixture:
		case youth_potion:
		case eagle_potion: return true;
		default: return false;
		}
	}

	static bool is_ingredient(object_type type) {
		switch (type) {
			using enum object_type;
		case no_ingredient:
		case start_brewing:
		case star_anise:
		case st_johns_wort:
		case spearmint:
		case solomons_seal:
		case thyme:
		case valerian:
		case wormwood:
		case sage:
		case patchouli:
		case hyssop:
		case majoram:
		case horehound: return true;
		default: return false;
		}
	}

	static std::vector<object_type> ingredients_for_treatment(object_type treatment) {
		switch (treatment) {
			using enum object_type;
		case bald_cure: return { object_type::star_anise, object_type::thyme };
		case barbarian_mix: return { object_type::wormwood, object_type::spearmint };
		case colorful_potion: return { object_type::st_johns_wort, object_type::sage };
		case thick_mixture: return { object_type::patchouli, object_type::hyssop };
		case youth_potion: return { object_type::solomons_seal, object_type::valerian };
		case eagle_potion:  return { object_type::majoram, object_type::horehound };
		default: return {};
		}
	}

	void update_y_off() {
		if (y_off_dir) {
			y_off += 0.1f;
			if (y_off > 3.0f) {
				y_off_dir = false;
			}
		} else {
			y_off -= 0.1f;
			if (y_off < -3.0f) {
				y_off_dir = true;
			}
		}
	}

};

class player_object : public game_object {
public:

	std::vector<object_type> items;
	std::vector<object_type> unlocked_cures;
	int gold{ 0 };
	
	player_object() : game_object{ object_type::player } {

	}

	void update(game_program& game) override;

	void remove_item(object_type item);

	void learn_cure(object_type cure);

};

enum class customer_state : unsigned char {
	waiting,
	in_treatment,
	after_treatment,
	leaving
};

class customer_object : public game_object {
public:

	customer_state state{ customer_state::waiting };

	std::optional<object_type> treated_with;
	bool reached_target{ false };
	bool ready_for_treatment{ false };
	object_type wanted_treatment{ object_type::bald_cure };

	std::optional<int> queue_position;
	std::optional<nfwk::vector2f> target_position;
	std::string greeting;

	customer_object(object_type type);

	void update(game_program& game) override;
	void set_type(object_type type) override;

	bool treat();

};

class brewing_stand_object : public game_object {
public:

	std::vector<game_object> items;
	nfwk::timer brew_timer;
	game_object result{ object_type::no_cure };
	nfwk::sprite_animation bubbles;

	brewing_stand_object() : game_object{ object_type::brewing_stand } {
		bubbles.frames = 6;
		bubbles.fps = 6.0f;
		bubbles.set_tex_coords({ 336.0f / 640.0f, 224.0f / 640.0f, 288.0f / 640.0f, 32.0f / 640.0f });
	}

	void set_result_from_items();

};
