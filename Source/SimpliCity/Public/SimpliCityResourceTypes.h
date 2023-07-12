// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpliCityResourceTypes.generated.h"

UENUM(BlueprintType)
enum class ESimpliCityResourceType : uint8 {
  None = 0,
  Bread = 1,
  Pancakes = 2,
  Donuts = 3,
  Chocolate = 4,
  Flour = 5,
  Soda = 6,
  Wine = 7,
  Sausage = 8,
  Coffee = 9,
  Popcorn = 10,
  HotDogs = 11,
  Milk = 12,
  Tea = 13,
  IceCream = 14,
  Chips = 15,
  Fish = 16,
  Wood = 17,
  Lumber = 18,
  Clay = 19,
  Brick = 20,
  Iron = 21,
  Steel = 22,
  Beer = 23,
  Squid = 24,
  OrangeJuice = 25,
  Burrito = 26,
  Quesadilla = 27,
  Jam = 28,
  Burger = 29,
  FrenchFries = 30,
  Ketchup = 31,
  Cheese = 32,
  TomatoSoup = 33,
  Power = 34,
  Water = 35,
  CoffeeBeans = 36,
  Corn = 37,
  Apples = 38,
  Oranges = 39,
  Bananas = 40,
  RedPeppers = 41,
  Potatoes = 42,
  Grapes = 43,
  TeaLeaves = 44,
  Wheat = 45,
  Garlic = 46,
  BellPepper = 47,
  Beef = 48,
  Peanuts = 49,
  Pumpkin = 50,
  Tomatoes = 51,
  TwoWayPaved = 52,
  TwoWayDirt = 53,
  Residential = 54,
  Commercial = 55,
  Industrial = 56
};

struct FSimpliCityRoadResource {
  static TArray<ESimpliCityResourceType> Types;
  static bool TryFindResource(ESimpliCityResourceType Type) {
    return Types.Contains(Type);
  }
};

struct FSimpliCityZoneResource {
  static TArray<ESimpliCityResourceType> Types;
  static bool TryFindResource(ESimpliCityResourceType Type) {
    return Types.Contains(Type);
  }
};

struct FSimpliCityFactoryResource {
  static TArray<ESimpliCityResourceType> Types;
  static bool TryFindResource(ESimpliCityResourceType Type) {
    return Types.Contains(Type);
  }
};

struct FSimpliCityFarmResource {
  static TArray<ESimpliCityResourceType> Types;
  static bool TryFindResource(ESimpliCityResourceType Type) {
    return Types.Contains(Type);
  }
};