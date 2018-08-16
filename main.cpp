/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: trungduc
 *
 * Created on August 16, 2018, 1:33 PM
 */

#include <cstdlib>
#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

vector<string> g_listLinks;

void getListLinks(string file);
void doDownload();
int main(int argc, char** argv) {
    getListLinks("hls.m3u8");
    doDownload();
    return 0;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void doDownload() { 
    string nameChunkCommond = "chunk";
    for(int i = 0; i < g_listLinks.size(); ++i) {
        CURL *curl;
        FILE *fp;
        CURLcode res;
        const char *url = g_listLinks[i].c_str();
        string name = nameChunkCommond + std::to_string(i);
        curl = curl_easy_init();
        if (curl) {
            fp = fopen(string("data/" + name).c_str(),"wb");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
            res = curl_easy_perform(curl);
            /* always cleanup */
            curl_easy_cleanup(curl);
            fclose(fp);
        }
    }
}

void getListLinks(string file) {
    string url;
    vector<string> listLinks;
    ifstream infile;
    infile.open(file.c_str());
    
    while (!infile.eof()) // To get you all the lines.
    {
        getline(infile, url); // Saves the line in url.
        if (url.find("http://") != std::string::npos)
        {
            g_listLinks.push_back(url);
        }
    }
    infile.close();
}


