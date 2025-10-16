#define CUTOFF 8

int *quicksort(int *data, int size);
void swap(int *a, int *b);
int partition(int *data, int size);

int *quicksort2(int *data, int size);
static void insertion_sort(int *a, int n);
int median3_index(int *a, int lo, int hi);
static int part_branchfree_cmov(int *a, int lo, int hi);
static int part_branchfree_xor(int *a, int lo, int hi);

// Use compiler-specific macro set at compile time
int part_branchfree(int *a, int lo, int hi) {
#ifdef USE_CMOV
    return part_branchfree_cmov(a, lo, hi);
#else
    return part_branchfree_xor(a, lo, hi);
#endif
}

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

// CMOV/ternary branch-free partition
// Uses conditional moves to avoid branches
static int part_branchfree_cmov(int *a, int lo, int hi) {
    int pivot = a[hi];
    int i = lo - 1;

    for (int j = lo; j < hi; j++) {
        // Branch-free comparison and conditional move
        int should_swap = (a[j] <= pivot);
        
        // Increment i conditionally
        int new_i = i + should_swap;
        
        // Perform swap using ternary operators (conditional moves)
        if (should_swap) {
            swap(&a[new_i], &a[j]);
        }
        
        i = new_i;
    }
    
    swap(&a[i + 1], &a[hi]);
    return i + 1;
}

// XOR/mask branch-free partition
// Uses bitwise operations and masks to avoid branches
static int part_branchfree_xor(int *a, int lo, int hi) {
    int pivot = a[hi];
    int i = lo - 1;

    for (int j = lo; j < hi; j++) {
        // Branch-free comparison: produces 0 or 1
        int cond = (a[j] <= pivot);
        
        // Extend to full mask: 0 or -1 (all bits set)
        int mask = -cond;
        
        // Increment i only when cond is true
        i += cond;
        
        // XOR-based swap: only swap if mask is -1 (all bits set)
        int temp_i = a[i];
        int temp_j = a[j];
        
        int t = (temp_i ^ temp_j) & mask;
        a[i] = temp_i ^ t;
        a[j] = temp_j ^ t;
    }
    
    swap(&a[i + 1], &a[hi]);
    return i + 1;
}

int *quicksort2(int *data, int size) {
    // Insertion sort on small partitions
    if (size <= CUTOFF) {
        insertion_sort(data, size);
        return data;
    }

    // Median-of-3 pivot selection
    int pivot_index = median3_index(data, 0, size - 1);
    swap(&data[pivot_index], &data[size - 1]);

    // Use branch-free partition (compiler-specific)
    int p = part_branchfree(data, 0, size - 1);

    // Recurse on both partitions
    quicksort2(data, p);
    quicksort2(data + p + 1, size - p - 1);

    return data;
}

// Standard insertion sort implementation
static void insertion_sort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int x = a[i];
        if (x >= a[i - 1])
            continue;
        int j = i - 1;
        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = x;
    }
}

// Standard median-of-3 pivot selection
int median3_index(int *a, int lo, int hi) {
    int mid = lo + ((hi - lo) >> 1);
    int x = a[lo], y = a[mid], z = a[hi];

    if (x < y) {
        if (y < z)
            return mid;
        else if (x < z)
            return hi;
        else
            return lo;
    } else {
        if (x < z)
            return lo;
        else if (y < z)
            return hi;
        else
            return mid;
    }
}