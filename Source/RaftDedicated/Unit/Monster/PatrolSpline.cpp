// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolSpline.h"

// Sets default values
APatrolSpline::APatrolSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SetRootComponent(m_Spline);

	m_DivideCount = 100;
	m_PrevDivide = 0;
	m_PrevLength = 0.f;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DebugAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	if (DebugAsset.Succeeded())
	{
		m_DebugMesh = DebugAsset.Object;
	}
}


//호출 시점이 드래고할때 배치하여 마우스를 놓을때 등 자주 호출된다
void APatrolSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ComputeSpline();
}

// Called when the game starts or when spawned
void APatrolSpline::BeginPlay()
{
	Super::BeginPlay();
	ComputeSpline();
}



// Called every frame
void APatrolSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APatrolSpline::ComputeSpline()
{
	m_PointArray.RemoveAll([](FVector& v) {return true;});
	m_RotatorArray.RemoveAll([](FRotator& v) {return true; });

	if (m_bMeshVisible)
	{
		if (m_DivideCount != m_PrevDivide)
		{
			for (int32 i = 0; i < m_MeshArray.Num(); ++i)
			{
				if (m_MeshArray[i])
					m_MeshArray[i]->DestroyComponent();
			}

			if (m_MeshArray.Num() > 0)
				m_MeshArray.RemoveAll([](UStaticMeshComponent*) {return true; });
		}
	}

	else
	{
		for (int32 i = 0; i < m_MeshArray.Num(); ++i)
		{
			if (m_MeshArray[i])
				m_MeshArray[i]->DestroyComponent();
		}

		if (m_MeshArray.Num() > 0)
			m_MeshArray.RemoveAll([](UStaticMeshComponent*) {return true; });
	}

	m_SplineLength = m_Spline->GetSplineLength();

	m_CellDistance = m_SplineLength / m_DivideCount;

	FTransform Transform = GetActorTransform();

	bool	MeshEmpty = m_MeshArray.Num() == 0;

	for (int32 i = 0; i <= m_DivideCount; ++i)
	{
		FVector Point = m_Spline->GetLocationAtDistanceAlongSpline(m_CellDistance * i,
			ESplineCoordinateSpace::World);

		m_PointArray.Add(Point);

		FVector LocalPoint = Transform.InverseTransformPosition(Point);

		FRotator Rot = m_Spline->GetRotationAtDistanceAlongSpline(m_CellDistance * i,
			ESplineCoordinateSpace::World);

		m_RotatorArray.Add(Rot);

		if (m_bMeshVisible)
		{
			if (m_DivideCount != m_PrevDivide || MeshEmpty)
			{
				UStaticMeshComponent* MeshCom = NewObject<UStaticMeshComponent>(this);

				FTransform MeshTransform = MeshCom->GetRelativeTransform();

				MeshTransform.SetLocation(LocalPoint);
				MeshTransform.SetScale3D(FVector(0.1f, 0.1f, 0.1f));

				MeshCom->SetRelativeTransform(MeshTransform);

				MeshCom->SetWorldRotation(Rot);

				MeshCom->SetStaticMesh(m_DebugMesh);

				m_MeshArray.Add(MeshCom);

				MeshCom->SetupAttachment(m_Spline);
			}

			else
			{
				m_MeshArray[i]->SetRelativeLocation(LocalPoint);
				m_MeshArray[i]->SetWorldRotation(Rot);
			}
		}
	}	

	m_PrevDivide = m_DivideCount;
	m_PrevLength = m_SplineLength;
}


