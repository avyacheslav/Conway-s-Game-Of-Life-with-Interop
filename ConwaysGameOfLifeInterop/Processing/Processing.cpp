#include <random>
#include <ctime>

const int worldSize = 30;

extern "C" __declspec(dllexport) auto GetWorldSize()
{
	return worldSize;
}

extern "C" __declspec(dllexport) void GetRandomData(int* data)
{
	srand((unsigned int)time(0));

	for (auto i = 0; i < worldSize; i++)
	{
		data[i] = rand() | rand() << 15;
	}
}

auto Increment = [](auto value) { return value == worldSize - 1 ? 0 : value + 1; };
auto Decrement = [](auto value) { return value == 0 ? worldSize - 1 : value - 1; };

int GetNeighbors(bool currentWorld[worldSize][worldSize], int row, int col)
{
	int result = 0;

	if (currentWorld[Increment(row)][Increment(col)]) result++;
	if (currentWorld[row][Increment(col)]) result++;
	if (currentWorld[Decrement(row)][Increment(col)]) result++;
	if (currentWorld[Decrement(row)][col])            result++;
	if (currentWorld[Decrement(row)][Decrement(col)]) result++;
	if (currentWorld[row][Decrement(col)]) result++;
	if (currentWorld[Increment(row)][Decrement(col)]) result++;
	if (currentWorld[Increment(row)][col])            result++;

	return result;
}

extern "C" __declspec(dllexport) void GetNextStep(int* data)
{
	bool currentWorld[worldSize][worldSize];
	bool newWorld[worldSize][worldSize];

	for (auto i = 0; i < worldSize; i++)
	{
		for (auto j = 0; j < worldSize; j++)
		{
			currentWorld[i][j] = false;
			newWorld[i][j] = false;
		}
	}

	if (data == nullptr) return;

	for (auto row = 0; row < worldSize; row++)
		for (auto col = 0; col < worldSize; col++)
		{
			
			if ((data[row] & (1 << col)) != 0)
			{
				currentWorld[row][col] = true;
			}
		}

	for (auto row = 0; row < worldSize; row++)
	{
		for (auto col = 0; col < worldSize; col++)
		{
			int neighbors = GetNeighbors(currentWorld, row, col);
			if (neighbors == 3 || currentWorld[row][col] == true && neighbors == 2)
			{
				newWorld[row][col] = true;
			}
			else
			{
				newWorld[row][col] = false;
			}
		}
	}

	for (auto i = 0; i < worldSize; i++)
	{
		data[i] = 0;
	}

	for (auto row = 0; row < worldSize; row++)
	{
		for (auto col = 0; col < worldSize; col++)
		{
			if (newWorld[row][col])
			{
				data[row] |= (1 << col);
			}
		}
	}
}