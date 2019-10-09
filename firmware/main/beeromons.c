#include "beeromons.h"
#include "url.h"
#include "cJSON.h"
#include "nokialcd.h"

typedef struct {
    char * pname;
    char * ptype;
    uint8_t id;
} bmpower_t;

typedef struct
{
    char * name;
    char * tname;
    char * pic;
    unsigned int id;
    unsigned int xp;
    unsigned int abv;
    unsigned int bubbles;
    unsigned int ml;
    bmpower_t ** pows;
} bm_t;

bm_t ** pool = NULL;
uint8_t pool_sz = 0;

bm_t * makebm(const cJSON* curr)
{
    bm_t * bm = (bm_t*)calloc(1,sizeof(bm_t));
    cJSON *elptr = NULL;
    cJSON *thisel = NULL;
//    char * current_key = curr->string;

    bm->id   =  cJSON_GetObjectItemCaseSensitive(curr,"id")->valueint;
    bm->name =  cJSON_GetObjectItemCaseSensitive(curr,"name")->valuestring;
    bm->tname =  cJSON_GetObjectItemCaseSensitive(curr,"type")->valuestring;
    bm->pic  =  cJSON_GetObjectItemCaseSensitive(curr,"pic")->valuestring;
    bm->abv  =  cJSON_GetObjectItemCaseSensitive(curr,"abv")->valueint;
    bm->bubbles =  cJSON_GetObjectItemCaseSensitive(curr,"bubbles")->valueint;
    bm->ml  =  cJSON_GetObjectItemCaseSensitive(curr,"ml")->valueint;
    bm->xp  =  cJSON_GetObjectItemCaseSensitive(curr,"xp")->valueint;

    thisel=cJSON_GetObjectItemCaseSensitive(curr,"powers");

/*    printf("\nin check %d\n\n",1);
    heap_caps_check_integrity_all(1);
*/

    bm->pows = NULL;
    int sz = cJSON_GetArraySize(thisel);
    printf("bm sz = %d\n",sz);



    bm->pows = (bmpower_t**)calloc(sz+1,sizeof(bmpower_t*));


/*    printf("\nin check %d\n\n",2);
    heap_caps_check_integrity_all(1);
*/

    int i=0;

    cJSON_ArrayForEach(elptr,thisel){
        bm->pows[i] = (bmpower_t*)calloc(1,sizeof(bmpower_t));
        bm->pows[i]->pname = cJSON_GetObjectItemCaseSensitive(elptr,"name")->valuestring;
        bm->pows[i]->ptype = cJSON_GetObjectItemCaseSensitive(elptr,"type")->valuestring;
        bm->pows[i]->id = cJSON_GetObjectItemCaseSensitive(elptr,"id")->valueint;
        i++;
    }

    bm->pows[i]= (bmpower_t*) NULL;

    return bm;
}

void printbm(bm_t * bm)
{
    printf("B %d %s %s %s %d %d %d %d\n",bm->id,bm->name,bm->tname,bm->pic,bm->abv,bm->bubbles,bm->ml,bm->xp);
    int i = 0;
    while(bm->pows[i]){
        printf(" P %d %s %s\n",bm->pows[i]->id,bm->pows[i]->pname,bm->pows[i]->ptype);
        i++;
    }
}

cJSON *  bmparser(char* jsonstring)
{
    bm_t* cbm;
    int i,j;
    if(pool != NULL){
        for(i=0;i<pool_sz;i++){
            j=0;
            cbm = pool[i];
            while(cbm->pows[j])
                free(cbm->pows[j++]);
            free(cbm);
        }
    }


    cJSON * pool_json = cJSON_Parse(jsonstring);

    heap_caps_check_integrity_all(1);

    cJSON * curr = pool_json->child;
    if(pool_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL){
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return NULL;
    }
    free(jsonstring);
    i=0;

    while(curr){
        i++;
        curr= curr->next;

    }
//    int nr = i;
    printf("nr of BM  :   %d\n",i);

    curr = pool_json->child;
    pool_sz = i;

    pool = (bm_t**)calloc(i, sizeof(bm_t*));
    printf("json %d el %p\n",i,pool);
    i = 0;
    while(curr){
        //   printf("\npre check %d\n\n",i);

        //heap_caps_check_integrity_all(1);
        *(pool+i)=makebm(curr);
        printbm(*(pool+i));
        curr=curr->next;
        i++;
    }

    return pool_json;
}

void get_beero()
{
    if(pool == NULL){
        needWifi(1);
        const char * dum = "a";
        char * json = postdata("/s/get_beeromon_info.php",1,(char**)&dum,(char**)&dum);
        bmparser(json);
    }

}


void beeromonGuiShelfL()
{
/*    const char * fields[2] = {"ab","ba"};
    const char * content[2]= {"123","aaaba"};
    needWifi(1);
    printf("beeromonGuiShelfL\n");
    char * json = postdata("/s/get_beeromon_info.php",2,(char**)fields,(char**)content);
*/
    get_beero();
    char * wrkstr = calloc(128,sizeof(char));

    lcd_clearB12(B12_WHITE);
    lcd_setLine(ROW_LENGTH>>1, 0            ,ROW_LENGTH>>1 , COL_HEIGHT   ,B12_BLACK );
    lcd_setLine(0            , COL_HEIGHT>>1,ROW_LENGTH    , COL_HEIGHT>>1,B12_BLACK );

    lcd_setStr(pool[0]->name,2,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"T:%s",pool[0]->tname);
    lcd_setStr(wrkstr,12,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"xp:%d",pool[0]->xp);
    lcd_setStr(wrkstr,22,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"ABV.:%d",pool[0]->abv);
    lcd_setStr(wrkstr,32,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"Bubbles:%d",pool[0]->bubbles);
    lcd_setStr(wrkstr,42,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"ml:%d",pool[0]->ml);
    lcd_setStr(wrkstr,52,2,B12_BLACK,B12_WHITE,0,0);

    lcd_setStr(pool[1]->name,2,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"T:%s",pool[1]->tname);
    lcd_setStr(wrkstr,12,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"xp:%d",pool[1]->xp);
    lcd_setStr(wrkstr,22,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"ABV.:%d",pool[1]->abv);
    lcd_setStr(wrkstr,32,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"Bubbles:%d",pool[1]->bubbles);
    lcd_setStr(wrkstr,42,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"ml:%d",pool[1]->ml);
    lcd_setStr(wrkstr,52,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);

    lcd_setStr(pool[2]->name,(COL_HEIGHT>>1)+2,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"T:%s",pool[2]->tname);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+12,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"xp:%d",pool[2]->xp);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+22,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"ABV.:%d",pool[2]->abv);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+32,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"Bubbles:%d",pool[2]->bubbles);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+42,2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"ml:%d",pool[2]->ml);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+52,2,B12_BLACK,B12_WHITE,0,0);

    lcd_setStr(pool[3]->name,(COL_HEIGHT>>1)+2,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"T:%s",pool[3]->tname);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+12,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"xp:%d",pool[3]->xp);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+22,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"ABV.:%d",pool[3]->abv);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+32,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"Bubbles:%d",pool[3]->bubbles);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+42,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);
    sprintf(wrkstr,"ml:%d",pool[3]->ml);
    lcd_setStr(wrkstr,(COL_HEIGHT>>1)+52,(ROW_LENGTH>>1)+2,B12_BLACK,B12_WHITE,0,0);


    lcd_sync();


}

void beeromonGuiFightL()
{
    get_beero();

}
