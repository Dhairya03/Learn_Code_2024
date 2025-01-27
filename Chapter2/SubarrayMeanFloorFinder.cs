using System;

class SubArrayMeanFloorFinder {
    static void Main(string[] args) {
        var userInput = ReadArrayInput();
        int totalArrayElements = userInput[0];
        int totalQueries = userInput[1];

        var arrayElements = ReadLongArrayInput();

        var arraySum = CalculateSum(arrayElements, totalArrayElements);

        ProcessQueries(totalQueries, arraySum);
    }

    static int[] ReadArrayInput() {
        return Array.ConvertAll(Console.ReadLine().Split(' '), int.Parse);
    }

    static long[] ReadLongArrayInput() {
        return Array.ConvertAll(Console.ReadLine().Split(' '), long.Parse);
    }

    static long[] CalculateSum(long[] arrayElements, int totalArrayElements) {
        long[] arraySum = new long[totalArrayElements + 1];
        arraySum[0] = 0;
        for (int indexCount = 1; indexCount <= totalArrayElements; indexCount++) {
            arraySum[indexCount] = arraySum[indexCount - 1] + arrayElements[indexCount - 1];
        }
        return arraySum;
    }

    static void ProcessQueries(int totalQueries, long[] arraySum) {
        for (int queryCount = 0; queryCount < totalQueries; queryCount++) {
            var query = ReadArrayInput();
            long mean = CalculateSubarrayMean(arraySum, query);
            Console.WriteLine(mean);
        }
    }

    static long CalculateSubarrayMean(long[] prefixSum,int[] query) {
        return ((long)(prefixSum[query[1]] - prefixSum[query[0] - 1]) / (query[1] - query[0] + 1));
    }
}
