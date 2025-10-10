#define CUTOFF 8

int *quicksort(int *data, int size);
void swap(int *a, int *b);
int partition(int *data, int size);

int *quicksort2(int *data, int size);
static void insertion_sort(int *a, int n);
int median3_index(int *a, int lo, int hi);

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* data, int size) {
    int pivot = data[size - 1]; 
    int i = -1;

    for (int j = 0; j < size - 1; j++) {
        if (data[j] <= pivot) {
            i++;
            swap(&data[i], &data[j]);
        }
    }
    swap(&data[i + 1], &data[size - 1]);
    return i + 1;
}

int* quicksort(int* data, int size) {
    if (size <= 1) return data;  

    int p = partition(data, size);

    quicksort(data, p);

    quicksort(data + p + 1, size - p - 1);

    return data;
}

int *quicksort2(int *data, int size)
{
    //insertion sort on size curoff
    if (size <= CUTOFF)
    {
        insertion_sort(data, size);
        return data;
    }

    //median of 3 pivot selection
    int pivot_index = median3_index(data, 0, size - 1);
    int pivot = data[pivot_index];

    //move pivot to last value to prepare for partition
    int temp = data[pivot_index];
    data[pivot_index] = data[size - 1];
    data[size - 1] = temp;
    

    //less than
    int lt = 0;

    //overall swap count
    int i = 0;
    
    //index of last element (excluding pivot)
    int gt = size - 2;

    while (i <= gt)
    {
        int x = data[i];
        int less    = (x < pivot);
        int greater = (!less) && (x > pivot);

        //handle less than
        int tmp_lt = data[lt];
        data[lt] = less ? data[i] : data[lt];
        data[i]  = less ? tmp_lt  : data[i];

        lt += less;
        i  += less;

        //handle greater than
        int mask = -greater;
        int t = (data[i] ^ data[gt]) & mask;
        data[i] ^= t;
        data[gt] ^= t;
        gt -= greater;

        //edge case where they're even
        int equal_advance = (!less && !greater);
        i += equal_advance;
    }

    //put pivot back
    temp = data[lt];
    data[lt] = data[size - 1];
    data[size - 1] = temp;

    //recurse
    quicksort2(data, lt);
    quicksort2(data + lt + 1, size - lt - 1);

    return data;
}

//standard insertion sort implementation
static void insertion_sort(int *a, int n)
{
    
    for (int i = 1; i < n; i++)
    {
        int x = a[i];
        if (x >= a[i - 1])
            continue;
        int j = i - 1;
        while (j >= 0 && a[j] > x)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = x;
    }
}

//standard median-of-3 pivot selection
int median3_index(int *a, int lo, int hi)
{
    int mid = lo + ((hi - lo) >> 1);
    int x = a[lo], y = a[mid], z = a[hi];

    if (x < y)
    {
        if (y < z)
            return mid;
        else if (x < z)
            return hi;
        else
            return lo;
    }
    else
    {
        if (x < z)
            return lo;
        else if (y < z)
            return hi;
        else
            return mid;
    }
}
