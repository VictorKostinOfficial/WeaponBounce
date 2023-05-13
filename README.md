# WeaponBounce

Тестовое задание

## Задание

1. Необходимо определить точку удара с объектом;
2. Обработать (Физически корректно) реакцию персонажа на удар по объекту в зависимости от угла удара;
3. Установить силу отскока прочитав Json файл;
4. Добавить звук удара и партиклы в момент удара.

## Идея

Для расчета отраженного вектора, нам нужен вектор удара и нормаль плоскости:

HitDir можно не нормализовывать, однако отраженный ветор тоже будет не нормализованный

>
> HitDir - Нормализованый вектор удара;
> 
> SurfaceNormal - Вектор нормали поверхности;
> 
> ReflectDir - Отраженный нормализованный вектор;
>
```
ReflectDir = HitDir - 2 * Dot(HitDir, SurfaceNormal) * SurfaceNormal;
```

Если вектор HitDir нормализован, то используя скалярное произведение можно найти проекцию `ReflectDir на SurfaceNormal`, что позволит нам определить под каким углом был сделан удар, острым или тупым.

```
Dot(SurfaceNormal, ReflectDir) == 1 : Удар под прямым углом
Dot(SurfaceNormal, ReflectDir) == 0 : Удар по касательной
```

<img width="300" height="260" alt="image" src="https://github.com/VictorKostinOfficial/WeaponBounce/assets/122555487/f474623d-4be1-492b-b2b5-6136a2195728">
<img width="300" height="260" alt="image" src="https://github.com/VictorKostinOfficial/WeaponBounce/assets/122555487/e4e6752e-4667-4fb0-8a8b-3f3a47a3a77d">
<img width="300" height="260" alt="image" src="https://github.com/VictorKostinOfficial/WeaponBounce/assets/122555487/f6ecc8bb-e283-4a88-966a-1bc90f76dad2">

## Реализация "Отскока"
При ударе об объект, я использовал параметр `SimulatePhysics`, но симулировал только верхнюю часть тела:
```
CharacterSkeletalMesh->SetAllBoddiesBelowSimulatePhysics(BoneName, ture, true);
```
Однако при симуляции из рук выпадал меч, прикрепил его через `PhysicsConstrain`. В таком случае меч не вылетает из руки при симуляции физики персонажа, а колизия отлавливает `Hit Event`:
> PhysicsConstrain - Связка между Box и Sword;
> 
> Box - Колизия с симуляцией физики для отлова `Hit Event`;
> 
> Sword - Модель меча, статичный меш;
<img width="413" alt="image" src="https://github.com/VictorKostinOfficial/WeaponBounce/assets/122555487/2d8792a9-5a24-47aa-936b-c5a8f4a21b54">


При тупом ударе я останавливал анимацию и умножал вектор `ReflectDir на WeaponBounce`, который находится в файле, при остром - анимацию и вектор не трогал, так как меч соскальзывает с поверхности.

Затем приминял полученный импульс к руке:
```
CharacterSkeletalMesh->AddImpulse(ReflectDirImpulse, "hand_r", false);
```

Полсе получения импульса, нужно отключить персонажу симуляцию физики. Для этого я использовал `HitReactionTimeRemaining`, параметр отвечает за время восстановления персонажа.
```
if (HitReactionTimeRemaining <= 0)
{
  HitReactionTimeRemaining = 0;
  CharacterSkeletalMesh->SetAllBodiesBelowSimulatePhysics(BoneName, true, true);
}
else
{
  FMath::FInterpTo(HitReactionTimeRemaining, 0.f, 0.f, FInterpSpeed);
  HitReactionTimeRemaining -= DeltaTime*DeltaTimeSpeed;
  CharacterSkeletalMesh->SetAllBodiesBelowPhysicsBlendWeight(BoneName, FMath::Min<float>(HitReactionTimeRemaining, 1.f), false, true);
}
```

## Json парсинг

Файл `MannequinParams.msg` лежит в /Game/Config/. Перед чтением файла проверим, существует ли он:
```
FString FilePath = FPaths::ProjectConfigDir().Append("MannequinParams.msg");
FString FileData = "";

// Если файл не существет
if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
{
    UE_LOG(LogTemp, Warning, TEXT("DID NOT FIND FILE"));
    return;
}
```

Чтение файла:
```
// Если файл не Json
if (!FFileHelper::LoadFileToString(FileData,*FilePath))
{
    UE_LOG(LogTemp, Warning, TEXT("FILE IS NOT LOADED"));
    return;
}
```
Конвертация Json файла:
```
TSharedPtr<FJsonObject> JsonParsed;
TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(FileData);

if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
{
    FString ResultJson = JsonParsed->GetStringField("WeaponBounceStr");
    WeaponBounce = FCString::Atof(*ResultJson);
    UE_LOG(LogTemp, Warning, TEXT("WeaponBounce: %f"), WeaponBounce);
}
else
{
    UE_LOG(LogTemp, Warning, TEXT("FILE DESERIALIZE FILED"));
}
```

## Звук и эффекты

Созданы 2 параметра:
```
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundCue* ImpactSound;
```

Вызваны в `Hit`
```
	if(ImpactVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, HitLocation);
	}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitLocation);
	}
```
