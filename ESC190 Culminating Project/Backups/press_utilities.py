import sys
from collections import OrderedDict
import numpy
import os
import csv

class entry:
    def __init__ (self, word, definition, author, popularity):
        self.word = str(word)
        self.definition = definition
        self.author = author
        self.popularity = int(popularity)

    def __str__ (self):
        return (self.word + "\nDefinition: " + self.definition + "\nAuthor: " + self.author + "\n")

def sort_dict(unsorted_dict):
    '''
    Sorts the dictionary in alphabetical order.
    '''
    N_Psi_dict = OrderedDict(sorted(unsorted_dict.items()))
    return N_Psi_dict

def insert_word(N_Psi_dict, word):
    '''
    Insert a new word with definition by an author.
    '''
    if(word in N_Psi_dict):
        while(True):
            confirm = input("That word already exists! Would you like to redefine it? (Y/N)\n").upper()
            if(confirm == "Y"):
                redefine_word(N_Psi_dict,word)
                break
            elif(confirm == "N"):
                break
    else:
        definition = input("Enter a definition for " + word + "\n")
        author = input("Enter author: \n")
        new_word = entry(word, definition, author, 0)
        N_Psi_dict[word] = new_word
    N_Psi_dict = sort_dict(N_Psi_dict)

def delete_word(N_Psi_dict,word):
    '''
    Deletes the word object. Returns -1 if no word with the word is found. Returns -1 if deletion is aborted.
    '''
    if(word not in N_Psi_dict):
        print("404: word not found.\n")
        return -1
    else:
        prompt = input("Are you sure you want to delete " + word + "? (Y/N)\n").upper()
        while(True):
            if(prompt == "Y"):
                print("word successfully deleted.\n")
                del N_Psi_dict[word]
                return 1
            elif(prompt == "N"):
                print("Deletion aborted.\n")
                return -1
            else:
                prompt = input().upper()

def redefine_word(N_Psi_dict, word):
    '''
    Redefines an existing word definition and author with a new definition and author.
    '''
    definition = input("Enter new word definition: \n")
    author = input("Enter new author: \n")
    if(word not in N_Psi_dict):
        print("404: word not found.\n")
        return -1
    else:
        prompt = input("Are you sure you want to redefine " + word + "? (Y/N)\n").upper()
        while(True):
            if(prompt == "Y"):
                del N_Psi_dict[word]
                new_word = entry(word, definition, author, 0)
                N_Psi_dict[new_word.word] = new_word
                return 1
            elif(prompt == "N"):
                print("Redefinement aborted\n")
                return -1
            else:
                prompt = input().upper

def search_for_word(N_Psi_dict, word):
    '''
    Returns True if word is found. Returns False if no word with the word is found.
    '''
    os.system("cls||clear")
    key_list = sorted(N_Psi_dict.keys())
    if (word not in N_Psi_dict):
        lev_list = {}
        last_word = entry("", "", "", 0)
        for near_word in key_list:
            if(near_word != word):
                lev_dist = levenshtein(word,near_word)
                if lev_dist not in lev_list.keys():
                    lev_list[lev_dist] = []
                lev_list[lev_dist].append(near_word)
                if(len(lev_list) > 3):
                    del lev_list[max(lev_list.keys())]
        if(min(lev_list.keys()) <= 3):
            final_list = lev_list[min(lev_list.keys())]
            for i in final_list:
                if (int(N_Psi_dict[i].popularity) >= last_word.popularity):
                    last_word = N_Psi_dict[i]
            confirm = input("Did you mean " + last_word.word + "? (Y/N)\n").upper()
            while(True):
                if (confirm == "Y"):
                    os.system("cls||clear")
                    N_Psi_dict[last_word.word].popularity += 1
                    print(last_word)
                    if(key_list.index(last_word.word) > 0 and key_list.index(last_word.word) < len(key_list) - 1):
                        next_word = key_list[key_list.index(last_word.word) + 1]
                        prev_word = key_list[key_list.index(last_word.word) - 1]
                        print("Previous word: " + prev_word + "\n")
                        print("Next word: " + next_word + "\n")
                    elif(key_list.index(last_word.word) == 0):
                        next_word = key_list[key_list.index(last_word.word) + 1]
                        print("Next word: " + next_word + "\n")
                    elif(key_list.index(last_word.word) == len(key_list) - 1):
                        prev_word = key_list[key_list.index(last_word.word) - 1]
                        print("Previous word: " + prev_word + "\n")
                    while(True):
                        scroll = input("Scroll with (prev/next/back)\n").lower()
                        if(scroll == "prev" and key_list.index(last_word.word) != 0):
                            search_for_word(N_Psi_dict, prev_word)
                            return True
                        elif(scroll == "next" and key_list.index(last_word.word) != len(key_list) - 1):
                            search_for_word(N_Psi_dict, next_word)
                            return True
                        elif(scroll == "back"):
                            return True
                elif (confirm == "N"):
                    print("404: word not found.\n")
                    return False
                else:
                    confirm = input("Please answer (Y/N).\n").upper()
        else:
            print("404: word not found.\n")
            return False
    else:
        os.system("cls||clear")
        N_Psi_dict[word].popularity += 1
        print(N_Psi_dict[word])
        if(key_list.index(word) > 0 and key_list.index(word) < len(key_list) - 1):
            print(key_list)
            next_word = key_list[key_list.index(word) + 1]
            prev_word = key_list[key_list.index(word) - 1]
            print("Previous word: " + prev_word + "\n")
            print("Next word: " + next_word + "\n")
        elif(key_list.index(word) == 0):
            next_word = key_list[key_list.index(word) + 1]
            print("Next word: " + next_word + "\n")
        elif(key_list.index(word) == len(key_list) - 1):
            prev_word = key_list[key_list.index(word) - 1]
            print("Previous word: " + prev_word + "\n")
        while(True):
            scroll = input("Scroll with (prev/next/back)\n").lower()
            if(scroll == "prev" and key_list.index(word) != 0):
                search_for_word(N_Psi_dict, prev_word)
                return True
            elif(scroll == "next" and key_list.index(word) != len(key_list) - 1):
                search_for_word(N_Psi_dict, next_word)
                return True
            elif(scroll == "back"):
                return True

def search_for_subset(N_Psi_dict, word, sub_range):
    if (word in N_Psi_dict) and sub_range > 0:
        return_list = [word]
        i = 1
        key_list = sorted(N_Psi_dict.keys())
        word_index = key_list.index(word)
        if (sub_range >= len(key_list)):
            print(str(key_list).strip("[]"))
            return key_list
        while i < sub_range:
            if(word_index + i < len(key_list) and i < sub_range):
                return_list.append(key_list[word_index + i])
                i += 1
            if(word_index - i >= 0 and i < sub_range):
                return_list.append(key_list[word_index - i])
                i += 1
        print("Subset: " + str(sorted(return_list)).strip("[]"))
        return sorted(return_list)
    else:
        print("404 word not found.\n")
        return False

def search_for_sequence(N_Psi_dict, sequence):
    key_list = sorted(N_Psi_dict.keys())
    return_list = []
    for i in key_list:
        if(i.find(sequence) > -1):
            return_list.append(i)
    if(return_list == []):
        print("Error 404 no words found.\n")
    print("Words that share that sequence: " + str(return_list).strip("[]") + "\n")
    return return_list

def levenshtein(word1,word2):
    x_length = len(word1) + 1
    y_length = len(word2) + 1
    array_2d = numpy.zeros((x_length, y_length))
    for i in range(x_length):
        array_2d[i, 0] = i
    for j in range(y_length):
        array_2d[0, j] = j
    for i in range(1, x_length):
        for j in range(1, y_length):
            if (word1[i-1] == word2[j-1]):
                array_2d[i,j] = min(array_2d[i-1, j] + 1, array_2d[i-1, j-1], array_2d[i, j-1] + 1)
            else:
                array_2d[i, j] = min(array_2d[i-1, j] + 1, array_2d[i-1, j-1] + 1, array_2d[i, j-1] + 1)
    return(array_2d[len(word1), len(word2)])

def load_save(N_Psi_dict):
    '''
    Reads attributes of word objects from a text file, and transports them to the N_Psi_dict data segment.
    '''
    if(input("Are you sure you want to load a new save? Current data will be lost. (Y/N)\n").upper() == "Y"):
        N_Psi_dict.clear()
        with open("dictionary.txt") as f:
            next(f)
            data = f.readlines()
            for line in data:
                dat = line.strip().split('\t')
                word, definition, author, popularity = dat[0], dat[1], dat[2], dat[3]
                N_Psi_dict[word] = entry(word, definition, author, popularity)
        return N_Psi_dict

def save(N_Psi_dict):
    '''
    Writes attributes of word objects from the N_Psi_dict data segment to a new save file.
    '''
    f = open("dictionary.txt", "w")
    N_Psi_dict = sort_dict(N_Psi_dict)
    f.write("Word\tDefinition\tAuthor\tNumber of Searches\n")
    for i in N_Psi_dict.values():
        f.write(str(i.word) + "\t" + str(i.definition) + "\t" + str(i.author) + "\t" + str(i.popularity) + "\n")
    f.close()