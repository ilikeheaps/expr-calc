void* median3(int (*comp)(void*, void*), void* el1, void* el2, void* el3)
{
    if(comp(el1, el2) < 0)
        if(comp(el2, el3) < 0)
            return el3;
        else
            if(comp(el1, el3) < 0)
                return el3;
            else
                return el1;
    else
        if(comp(el1, el3) < 0)
            return el1;
        else
            if(comp(el2, el3) < 0)
                return el3;
            else
                return el2;
}

void swap(void** p, void** q)
{
    void* t = *p;
    *p = *q;
    *q = t;
}

void partition(void*** ret_first_pivot, void*** ret_last_pivot, int (*comp)(void*, void*), void** begin, void** end)
{
    /*
     * comp(p1, p2) > 0 : p1 > p2
     * comp(p1, p2) = 0 : p1 = p2
     * comp(p1, p2) < 0 : p1 < p2
     */
    
    void *piv = median3(comp, *begin, *(begin + (end - begin)/2), *end);
    
    void **p = begin;
    void **q = end;
    
    while(comp(*p, piv) < 0)
        p++;
        
    while(comp(*q, piv) >= 0)
        q--;
    
    while(comp(*p, *q) < 0)
    {
        swap(p, q);
        
        while(comp(*p, piv) < 0)
            p++;
        
        while(comp(*q, piv) >= 0)
            q--;
    }
    
    *ret_first_pivot = p;
    
    
    q = end;
    
    while(comp(*p, piv) == 0)
        p++;
        
    while(comp(*q, piv) > 0)
        q--;
    
    while(comp(*p, *q) < 0)
    {
        swap(*p, *q);
        
        while(comp(*p, piv) == 0)
            p++;
        
        while(comp(*q, piv) == 0)
            q--;
    }
    
    *ret_last_pivot = p - 1;
}

void** minimum(int (*comp)(void*, void*), void** begin, void** end)
{
    void** minimum = begin;
     
    for(void** p = begin + 1; p <= end; p++)
        if(comp(*minimum, *p) > 0)
            minimum = p;
        
    return minimum;
}

void insertSort(int (*comp)(void*, void*), void** begin, void** end)
{
    for(void** p = begin; p < end; p++)
        swap(minimum(comp, p, end), p);
}

void quickSort(int (*comp)(void*, void*), void** begin, void** end)
{
    void** last_piv;
    void** first_piv;
    
    if(end - begin > 10)
    {
        partition(&first_piv, &last_piv, comp, begin, end);
        
        quickSort(comp, begin, first_piv - 1);
        quickSort(comp, last_piv + 1, end);
    }
    else
        insertSort(comp, begin, end);
}