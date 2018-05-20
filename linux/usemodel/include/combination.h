/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
 *
 ***************************************************************************
 */
//combination.h

#ifndef __COMBINATION_H__
#define __COMBINATION_H__

// Non recursive template function
template <class BidIt>

bool next_combination(BidIt n_begin, BidIt n_end,BidIt r_begin, BidIt r_end)
{
  bool boolmarked=false;
  BidIt r_marked;
  
  BidIt n_it1=n_end;
  --n_it1;
  
  
  BidIt tmp_r_end=r_end;
  --tmp_r_end;
  BidIt tmp_r_begin=r_begin;
  
  BidIt r_it1;
  for(r_it1=tmp_r_end; r_it1>=tmp_r_begin ; --r_it1,--n_it1)
  {
    if(*r_it1==*n_it1 )
    {
      if(r_it1!=r_begin) //to ensure not at the start of r sequence
      {
        boolmarked=true;
        r_marked=(--r_it1);
	    ++r_it1;//add it back again 
        continue;
      }
      else // it means it is at the start the sequence, so return false
	  {
			return false;  
	  }   
	
    }
    else //if(*r_it1!=*n_it1 )
    {
      //marked code
      if(boolmarked==true)
      {
        //for loop to find which marked is in the first sequence
        BidIt n_marked;//mark in first sequence
		BidIt n_it2 ;
		bool found = false;
        for (n_it2=n_begin;n_it2!=n_end;++n_it2)
          if(*r_marked==*n_it2) {n_marked=n_it2;found = true;break;}
      
		
		if(found == false) return false;
		found = false;
        BidIt n_it3=++n_marked;   
		BidIt r_it2 ;
        for  (r_it2=r_marked;r_it2!=r_end&&n_it3!=n_end;++r_it2,++n_it3)
        {
          *r_it2=*n_it3;
		  found = true;
        }
		if(found == false) return false;
        return true;
      }
	  
	  BidIt n_it4;
      for(n_it4=n_begin; n_it4<n_end; ++n_it4)
        if(*r_it1==*n_it4)
        {
          *r_it1=*(++n_it4);
          return true;           
        }
    }
  }  
  return false;//will never reach here    
}



// Non recursive template function
template <class BidIt>

bool prev_combination(BidIt n_begin, BidIt n_end,
BidIt r_begin, BidIt r_end)
{
  
  bool boolsame=false;
  BidIt marked;//for r
  BidIt r_marked;
  BidIt n_marked;
  

  BidIt tmp_n_end=n_end;
  --tmp_n_end;
  BidIt tmp_n_begin=n_begin;
  --tmp_n_begin;
  
  BidIt r_it1=r_end;
  --r_it1;
  
  for(BidIt n_it1=tmp_n_end; n_it1!=tmp_n_begin ; --n_it1)
  {
    if(*r_it1==*n_it1)
    {
      r_marked=r_it1;
      n_marked=n_it1;
      break;
    }
  }  
  
  BidIt n_it2=n_marked;


  BidIt tmp_r_end=r_end;
  --tmp_r_end;
  BidIt tmp_r_begin=r_begin;
  --tmp_r_begin;
  
  for(BidIt r_it2=r_marked; r_it2!=tmp_r_begin; --r_it2,--n_it2)
  {
    if(*r_it2==*n_it2 )
    {
      if(r_it2==r_begin&&*r_it2!=*n_begin)
      {
        for(BidIt n_it3=n_begin;n_it3!=n_end;++n_it3)
        {
          if(*r_it2==*n_it3)
          {
            marked=r_it2;
            *r_it2=*(--n_it3);
            
            BidIt n_it4=n_end;
            --n_it4;
            for(BidIt r_it3=tmp_r_end; r_it3!=tmp_r_begin &&r_it3!=marked; --r_it3,--n_it4)
            {
              *r_it3=*n_it4;              
            }
            return true;
          }
        }
      }
      else if(r_it2==r_begin&&*r_it2==*n_begin)
      {
        return false;//no more previous combination; 
      }
    }
    else //if(*r_it2!=*n_it2 )
    {
      ++r_it2;
      marked=r_it2;
      for(BidIt n_it5=n_begin;n_it5!=n_end;++n_it5)
      {
        if(*r_it2==*n_it5)
        {
          *r_it2=*(--n_it5);

          BidIt n_it6=n_end;
          --n_it6;
          for(BidIt r_it4=tmp_r_end; r_it4!=tmp_r_begin &&r_it4!=marked; --r_it4,--n_it6)
          {
            *r_it4=*n_it6;              
          }
          return true;
        }
      }
    }
  }  
 
  return false;//Will never reach here, unless error    
}




// Recursive template function
template <class RanIt, class Func>

void recursive_combination(RanIt nbegin, RanIt nend, int n_column,
		      RanIt rbegin, RanIt rend, int r_column,int loop, Func func)
{
	
	int r_size=rend-rbegin;
	
	
	int localloop=loop;
	int local_n_column=n_column;
	
	//A different combination is out
	if(r_column>(r_size-1))
	{
    func(rbegin,rend);
    return;
	}
	/////////////////////////////////
	
	for(int i=0;i<=loop;++i)
	{
				
		RanIt it1=rbegin;
		for(int cnt=0;cnt<r_column;++cnt)
		{
		  ++it1;
		} 
		
		RanIt it2=nbegin;
		for(int cnt2=0;cnt2<n_column+i;++cnt2)
		{
		  ++it2;
		} 
		
		*it1=*it2;
		
		++local_n_column;
		
		recursive_combination(nbegin,nend,local_n_column,
		        rbegin,rend,r_column+1,localloop,func);
		--localloop;
	}
	
}

#endif