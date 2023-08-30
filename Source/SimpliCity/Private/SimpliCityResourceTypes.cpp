// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpliCityResourceTypes.h"
TArray<ESimpliCityResourceType> FSimpliCityRoadResource::Types = {
  ESimpliCityResourceType::TwoWayPaved,
  ESimpliCityResourceType::TwoWayDirt
};

TArray<ESimpliCityResourceType> FSimpliCityZoneResource::Types = {
  ESimpliCityResourceType::Residential,
  ESimpliCityResourceType::Commercial,
  ESimpliCityResourceType::Industrial
};

TArray<ESimpliCityResourceType> FSimpliCityFactoryResource::Types = {
  ESimpliCityResourceType::Bread,
  ESimpliCityResourceType::Power,
  ESimpliCityResourceType::Water
};

TArray<ESimpliCityResourceType> FSimpliCityFarmResource::Types = {
  ESimpliCityResourceType::Wheat
 };
