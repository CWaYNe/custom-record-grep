#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <time.h>
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>
#include "load_data.h"
#include "search.h"


using namespace std;

inline bool exists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int isDir(const std::string &name) {
	struct stat buffer;
	stat(name.c_str(), &buffer);
	if ( !S_ISDIR(buffer.st_mode) )
	{
		return 0;
	}
	return 1;
}

int loadData(string path, struct record *&data) {
	FILE *fptr;
	char *line = NULL;
	char prefix[5];
	size_t len = 0;
	ssize_t read;
	int dataCount = 0, fileCount = 0;
	DIR *dir;
	struct dirent *ent;
	std::vector<string> files;
	std::vector<string> tagFiles;

	struct record *moreData = NULL;
	if (exists(path)){
		if(isDir(path)){
			if ((dir = opendir(path.c_str())) != NULL) {
				if (path[path.length()-1] != '/')
					path = path+"/";
				while ((ent = readdir(dir)) != NULL) {
					string newFilePath = path+ent->d_name;
					if (!isDir(newFilePath)) {
						files.push_back(newFilePath);
						tagFiles.push_back(newFilePath.substr(0, newFilePath.size()-3)+"info");
						fileCount++;
					}
				}
				closedir(dir);
			}
		}
		else {
			files.push_back(path);
			tagFiles.push_back(path.substr(0, path.size()-3)+"info");
			fileCount++;
		}
	}else {
		cout << "File doesn't exists !" << endl;
		exit(1);
	}
	cout << tagFiles[0] << endl;

	// NOTE: A better way to handle format inconsistency
	for (int i=0; i<fileCount; i++){
		fptr = fopen(files[i].c_str(), "r");
		while((read = getline(&line, &len, fptr)) != -1){ //char **restrict lineptr
			memcpy( prefix, line, 4);
			prefix[4] = '\0';
			if(strcmp(prefix, "@id:")==0){
					dataCount++;
					moreData = (struct record *) realloc(data, dataCount*sizeof(struct record));
					if (moreData != NULL){
						data = moreData;
						// data[dataCount-1].score = 0;
						data[dataCount-1].id = (char *) malloc(read-4);
						strcpy(data[dataCount-1].id, (line+4));
						// cout << (line+4);
						// printf("line: %s\n", line);
					}else {
						free(data);
						puts ("Error (re)allocating memory");
	       		exit (1);
					}
			}else if (strcmp(prefix, "@tit")==0){
					data[dataCount-1].title = (char *) malloc(read-7);
					strcpy(data[dataCount-1].title, (line+7));
			}else if (strcmp(prefix, "@con")==0){
					data[dataCount-1].content = (char *) malloc(read-9);
					strcpy(data[dataCount-1].content, (line+9));
			}else {
					cout << "File " +files[i]+ " did not obeyed input format" << endl;
					break;
			}
		}
		fclose(fptr);
		//Ranking recordRank(tagFiles[i]);
		//recordRank.buildRank();
		Ranking *recordRank  = new Ranking(tagFiles[i]);
		recordRank->buildRank();
	}

	return dataCount;
}

int loadData2(string path, struct record **data) {
	FILE *fptr;
	char *line = NULL;
	char prefix[5];
	size_t len = 0;
	ssize_t read;
	int dataCount = 0, fileCount = 0;
	DIR *dir;
	struct dirent *ent;
	std::vector<string> files;
	struct record *moreData = NULL;

	if (exists(path)){
		if(isDir(path)){
			if ((dir = opendir(path.c_str())) != NULL) {
				if (path[path.length()-1] != '/')
					path = path+"/";
				while ((ent = readdir(dir)) != NULL) {
					string newFilePath = path+ent->d_name;
					if (!isDir(newFilePath)) {
						files.push_back(newFilePath);
						fileCount++;
					}
				}
				closedir(dir);
			}
		}
		else {
			files.push_back(path);
			fileCount++;
		}
	}else {
		cout << "File doesn't exists !" << endl;
		exit(1);
	}

	// NOTE: A better way to handle format inconsistency
	for (int i=0; i<fileCount; i++){
		fptr = fopen(files[i].c_str(), "r");
		while((read = getline(&line, &len, fptr)) != -1){ //char **restrict lineptr
			memcpy( prefix, line, 4);
			prefix[4] = '\0';
			if(strcmp(prefix, "@id:")==0){
					dataCount++;
					moreData = (struct record *) realloc(*data, dataCount*sizeof(struct record));
					if (moreData != NULL){
						*data = moreData;
						// (*data+dataCount-1)->score = 0;
						(*data+dataCount-1)->id = (char *) malloc(read-4);
						strcpy((*data+dataCount-1)->id, (line+4));
					}else {
						free(*data);
						puts ("Error (re)allocating memory");
	       		exit (1);
					}
			}else if (strcmp(prefix, "@tit")==0){
					(*data+dataCount-1)->title = (char *) malloc(read-7);
					strcpy((*data+dataCount-1)->title, (line+7));
			}else if (strcmp(prefix, "@con")==0){
					(*data+dataCount-1)->content = (char *) malloc(read-9);
					strcpy((*data+dataCount-1)->content, (line+9));
			}else {
					cout << "File " +files[i]+ " did not obeyed input format" << endl;
					break;
			}
		}
		fclose(fptr);
	}

	return dataCount;
}