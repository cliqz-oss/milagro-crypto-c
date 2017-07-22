/**
 * @file test_mpinfull_random_ZZZ.c
 * @author Kealan McCusker
 * @brief Test Mpin-full good token and correct PIN with D-TA
 *
 * LICENSE
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* Test good token and correct PIN with D-TA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pbc_support.h"
#include "mpin_ZZZ.h"
#include "randapi.h"
#include "config_test.h"

// Define PIN range:
#define MAX_RANGE 10000

void rand_str(char *dest, size_t length,csprng *RNG)
{
    BIG_XXX r;
    char charset[] = "0123456789@.*"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    while (length-- > 0)
    {
        BIG_XXX_random(r,RNG);
        size_t index = r[0] % (sizeof charset);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

int main()
{
    int PIN1,PIN2,rtn,err,iter;

    char id[256];
    octet ID = {0,sizeof(id),id};

    char x[PGS_ZZZ],y[PGS_ZZZ];
    octet X= {sizeof(x), sizeof(x),x};
    octet Y= {sizeof(y),sizeof(y),y};

    /* Master secret shares */
    char ms1[PGS_ZZZ], ms2[PGS_ZZZ];
    octet MS1= {sizeof(ms1),sizeof(ms1),ms1};
    octet MS2= {sizeof(ms2),sizeof(ms2),ms2};

    /* Hash values of client ID */
    char hcid[PFS_ZZZ];
    octet HCID= {sizeof(hcid),sizeof(hcid), hcid};

    /* Hash values of messages */
    char hm[PFS_ZZZ];
    octet HM= {sizeof(hm),sizeof(hm), hm};

    /* Client secret and shares */
    char cs1[2*PFS_ZZZ+1], cs2[2*PFS_ZZZ+1], sec[2*PFS_ZZZ+1];
    octet SEC= {sizeof(sec),sizeof(sec),sec};
    octet CS1= {sizeof(cs1),sizeof(cs1), cs1};
    octet CS2= {sizeof(cs2),sizeof(cs2), cs2};

    /* Server secret and shares */
    char ss1[4*PFS_ZZZ], ss2[4*PFS_ZZZ], serverSecret[4*PFS_ZZZ];
    octet ServerSecret= {sizeof(serverSecret),sizeof(serverSecret),serverSecret};
    octet SS1= {sizeof(ss1),sizeof(ss1),ss1};
    octet SS2= {sizeof(ss2),sizeof(ss2),ss2};

    /* Time Permit and shares */
    char tp1[2*PFS_ZZZ+1], tp2[2*PFS_ZZZ+1], tp[2*PFS_ZZZ+1];
    octet TP= {sizeof(tp),sizeof(tp),tp};
    octet TP1= {sizeof(tp1),sizeof(tp1),tp1};
    octet TP2= {sizeof(tp2),sizeof(tp2),tp2};

    /* Token stored on computer */
    char token[2*PFS_ZZZ+1];
    octet TOKEN= {sizeof(token),sizeof(token),token};

    /* Precomputed values stored on device */
    char g1[12*PFS_ZZZ],g2[12*PFS_ZZZ];
    octet G1= {0,sizeof(g1),g1};
    octet G2= {0,sizeof(g2),g2};

    char ut[2*PFS_ZZZ+1],u[2*PFS_ZZZ+1];
    octet UT= {sizeof(ut),sizeof(ut),ut};
    octet U= {sizeof(u),sizeof(u),u};

    char hid[2*PFS_ZZZ+1],htid[2*PFS_ZZZ+1];
    octet HID= {0,sizeof(hid),hid};
    octet HTID= {0,sizeof(htid),htid};

    char e[12*PFS_ZZZ], f[12*PFS_ZZZ];
    octet E= {sizeof(e),sizeof(e),e};
    octet F= {sizeof(f),sizeof(f),f};

    char r[PGS_ZZZ],z[2*PFS_ZZZ+1],w[PGS_ZZZ],t[2*PFS_ZZZ+1];
    char ck[PAS_ZZZ],sk[PAS_ZZZ];
    octet R= {0,sizeof(r),r};
    octet Z= {0,sizeof(z),z};
    octet W= {0,sizeof(w),w};
    octet T= {0,sizeof(t),t};
    octet SK= {0,sizeof(sk),sk};
    octet CK= {0,sizeof(ck),ck};

    octet *pID;

    int date = 0;

    int byte_count = 32;
    char seed[32] = {0};
    octet SEED = {sizeof(seed),sizeof(seed),seed};
    csprng RNG;
#ifdef __linux__
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    if (fread(&seed, 1, byte_count, fp)) {};
    fclose(fp);
#else
    /* non random seed value! */
    int i;
    unsigned long ran;
    time((time_t *)&ran);
    SEED.val[0]=ran;
    SEED.val[1]=ran>>8;
    SEED.val[2]=ran>>16;
    SEED.val[3]=ran>>24;
    for (i=4; i<byte_count; i++) SEED.val[i]=i+1;
#endif
    printf("SEED 0x");
    OCT_output(&SEED);

    /* initialise random number generator */
    CREATE_CSPRNG(&RNG,&SEED);

    for(iter=1; iter<nRandomTests+1; iter++)
    {
        /* Generate Client master secret for MIRACL and Customer */
        rtn = MPIN_ZZZ_RANDOM_GENERATE(&RNG,&MS1);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_RANDOM_GENERATE(&RNG,&MS1) Error %d\n", rtn);
            return 1;
        }
        rtn = MPIN_ZZZ_RANDOM_GENERATE(&RNG,&MS2);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_RANDOM_GENERATE(&RNG,&MS2) Error %d\n", rtn);
            return 1;
        }
        printf("MASTER SECRET MIRACL:= 0x");
        OCT_output(&MS1);
        printf("MASTER SECRET CUSTOMER:= 0x");
        OCT_output(&MS2);

        /* Generate server secret shares */
        rtn = MPIN_ZZZ_GET_SERVER_SECRET(&MS1,&SS1);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_GET_SERVER_SECRET(&MS1,&SS1) Error %d\n", rtn);
            return 1;
        }
        rtn = MPIN_ZZZ_GET_SERVER_SECRET(&MS2,&SS2);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_GET_SERVER_SECRET(&MS2,&SS2) Error %d\n", rtn);
            return 1;
        }
        printf("SS1 = 0x");
        OCT_output(&SS1);
        printf("SS2 = 0x");
        OCT_output(&SS2);

        /* Combine server secret share */
        rtn = MPIN_ZZZ_RECOMBINE_G2(&SS1, &SS2, &ServerSecret);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_RECOMBINE_G2(&SS1, &SS2, &ServerSecret) Error %d\n", rtn);
            return 1;
        }
        printf("ServerSecret = 0x");
        OCT_output(&ServerSecret);

        /* Assign the End-User an ID */
        rand_str(id,255,&RNG);
        printf("CLIENT: ID %s\n", id);

        srand(time(NULL));
        PIN1 = rand()%MAX_RANGE; // Get random between 0 and MAX_RANGE
        PIN2 = PIN1;
        printf("PIN1 %d PIN2 %d\n", PIN1, PIN2);

        /* Hash ID */
        HASH_ID(HASH_TYPE_MPIN_ZZZ,&ID,&HCID);
        OCT_output(&HCID);

        /* When set only send hashed IDs to server */
#ifdef USE_ANONYMOUS
        pID = &HCID;
#else
        pID = &ID;
#endif

        /* Generate client secret shares */
        rtn = MPIN_ZZZ_GET_CLIENT_SECRET(&MS1,&HCID,&CS1);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_GET_CLIENT_SECRET(&MS1,&HCID,&CS1) Error %d\n", rtn);
            return 1;
        }
        rtn = MPIN_ZZZ_GET_CLIENT_SECRET(&MS2,&HCID,&CS2);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_GET_CLIENT_SECRET(&MS2,&HCID,&CS2) Error %d\n", rtn);
            return 1;
        }
        printf("CS1 = 0x");
        OCT_output(&CS1);
        printf("CS2 = 0x");
        OCT_output(&CS2);

        /* Combine client secret shares : TOKEN is the full client secret */
        rtn = MPIN_ZZZ_RECOMBINE_G1(&CS1, &CS2, &TOKEN);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_RECOMBINE_G1(&CS1, &CS2, &TOKEN) Error %d\n", rtn);
            return 1;
        }
        printf("Client Secret = 0x");
        OCT_output(&TOKEN);

        /* Generate Time Permit shares */
        date = today();
        printf("Date %d \n", date);
        rtn = MPIN_ZZZ_GET_CLIENT_PERMIT(HASH_TYPE_MPIN_ZZZ,date,&MS1,&HCID,&TP1);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_GET_CLIENT_PERMIT(HASH_TYPE_MPIN_ZZZ,date,&MS1,&HCID,&TP1) Error %d\n", rtn);
            return 1;
        }
        rtn = MPIN_ZZZ_GET_CLIENT_PERMIT(HASH_TYPE_MPIN_ZZZ,date,&MS2,&HCID,&TP2);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_GET_CLIENT_PERMIT(HASH_TYPE_MPIN_ZZZ,date,&MS2,&HCID,&TP2) Error %d\n", rtn);
            return 1;
        }
        printf("TP1 = 0x");
        OCT_output(&TP1);
        printf("TP2 = 0x");
        OCT_output(&TP2);

        /* Combine Time Permit shares */
        rtn = MPIN_ZZZ_RECOMBINE_G1(&TP1, &TP2, &TP);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_RECOMBINE_G1(&TP1, &TP2, &TP) Error %d\n", rtn);
            return 1;
        }
        printf("Time Permit = 0x");
        OCT_output(&TP);

        /* This encoding makes Time permit look random */
        if (MPIN_ZZZ_ENCODING(&RNG,&TP)!=0) printf("Encoding error\n");
        printf("Encoded Time Permit= ");
        OCT_output(&TP);
        if (MPIN_ZZZ_DECODING(&TP)!=0) printf("Decoding error\n");
        printf("Decoded Time Permit= ");
        OCT_output(&TP);

        /* Client extracts PIN1 from secret to create Token */
        rtn = MPIN_ZZZ_EXTRACT_PIN(HASH_TYPE_MPIN_ZZZ,&ID, PIN1, &TOKEN);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_EXTRACT_PIN( &ID, PIN, &TOKEN) Error %d\n", rtn);
            return 1;
        }
        printf("Token = 0x");
        OCT_output(&TOKEN);

        /* Client precomputation */
        MPIN_ZZZ_PRECOMPUTE(&TOKEN,&HCID,NULL,&G1,&G2);

        /* Client first PAS_ZZZs */
        rtn = MPIN_ZZZ_CLIENT_1(HASH_TYPE_MPIN_ZZZ,date,&ID,&RNG,&X,PIN2,&TOKEN,&SEC,&U,&UT,&TP);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_CLIENT_1 ERROR %d\n", rtn);
            return 1;
        }

        /* Client sends Z=r.ID to Server */
        MPIN_ZZZ_GET_G1_MULTIPLE(&RNG,1,&R,&HCID,&Z);

        /* Server calculates H(ID) and H(T|H(ID)) (if time permits enabled), and maps them to points on the curve HID and HTID resp. */
        MPIN_ZZZ_SERVER_1(HASH_TYPE_MPIN_ZZZ,date,pID,&HID,&HTID);

        /* Server generates Random number Y and sends it to Client */
        rtn = MPIN_ZZZ_RANDOM_GENERATE(&RNG,&Y);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_RANDOM_GENERATE(&RNG,&Y) Error %d\n", rtn);
            return 1;
        }
        printf("Y = 0x");
        OCT_output(&Y);

        /* Server sends T=w.ID to client */
        MPIN_ZZZ_GET_G1_MULTIPLE(&RNG,0,&W,&HTID,&T);
        printf("T = 0x");
        OCT_output(&T);

        /* Client second PAS_ZZZs */
        rtn = MPIN_ZZZ_CLIENT_2(&X,&Y,&SEC);
        if (rtn != 0)
        {
            printf("MPIN_ZZZ_CLIENT_2(&X,&Y,&SEC) Error %d\n", rtn);
        }
        printf("V = 0x");
        OCT_output(&SEC);

        /* Server second PAS_ZZZs */
#ifdef USE_DVS
        rtn = MPIN_ZZZ_SERVER_2(date,&HID,&HTID,&Y,&ServerSecret,&U,&UT,&SEC,&E,&F,NULL);
#else
        rtn = MPIN_ZZZ_SERVER_2(date,&HID,&HTID,&Y,&ServerSecret,&U,&UT,&SEC,&E,&F);
#endif

        if (rtn != 0)
        {
            err=MPIN_ZZZ_KANGAROO(&E,&F);
            if (err==0)
            {
                printf("FAILURE Invalid Token Error Code %d\n", rtn);
            }
            else
            {
                printf("FAILURE PIN Error %d, Error Code %d\n",err, rtn);
            }
        }

        HASH_ALL(HASH_TYPE_MPIN_ZZZ,&HCID,&U,&UT,&SEC,&Y,&Z,&T,&HM);
        MPIN_ZZZ_CLIENT_KEY(HASH_TYPE_MPIN_ZZZ,&G1,&G2,PIN2,&R,&X,&HM,&T,&CK);
        printf("Client Key = ");
        OCT_output(&CK);

        /* Server will use the hashed ID if anonymous connection required.
        HASH_ID(HASH_TYPE_MPIN_ZZZ,&ID,&HSID);
        HASH_ALL(HASH_TYPE_MPIN_ZZZ,&HSID,NULL,&UT,&SEC,&Y,&Z,&T,&HM);
        */
        MPIN_ZZZ_SERVER_KEY(HASH_TYPE_MPIN_ZZZ,&Z,&ServerSecret,&W,&HM,&HID,NULL,&UT,&SK);
        printf("Server Key = ");
        OCT_output(&SK);

        if (!OCT_comp(&CK,&SK))
        {
            printf("Iteration %d FAILURE keys are different\n", iter);
            return 1;
        }
        else
        {
            printf("Iteration %d SUCCESS\n\n", iter);
        }
    }
    return 0;
}
