#include "Core/ResourcePool.h"
#include "TestObject.h"

using namespace Hydrogen;

int main(int argc, char** arvg)
{


	Hydrogen::ResourcePool<Object> Pool;

	Id ID1 = Pool.Create(new Object());
	Id ID2 = Pool.Create(new Object());

	Pool.GetResource(ID1).Do(ID1);
	Pool.GetResource(ID1).Do(ID1);
	Pool.GetResource(ID1).Do(ID1);
	Pool.GetResource(ID1).Do(ID1);



	Pool.Destroy(&ID2);

	std::cin.get();
	return 0;
}