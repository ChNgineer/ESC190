import sys
from press_utilities_final import sort_dict, insert_word, redefine_word, delete_word, search_for_word, search_for_subset, search_for_sequence, load_save, save
import os

N_Psi_dict = {}
authors = {}

if(__name__ == "__main__"):
    save_state = 0
    print(sys.version)
    while(True):
        os.system("cls||clear")
        user = input("Enter a command:\n"
                            "======================\n"
                            "insert\t\tdelete\n"
                            "redefine\tsearch\n"
                            "load\t\tsave\n"
                            "size\t\tquit\n"
                            "======================\n"
                            "authors: " + str(list(authors.keys())).strip("[]") + "\n")

        if(user.lower() == "insert"):
            print("Inserting... Please follow the prompts.\n")
            word = input("Enter new word : \n").capitalize()
            insert_word(N_Psi_dict, authors, word)
            N_Psi_dict = sort_dict(N_Psi_dict)
            os.system('cls||clear')
            save_state = 0

        elif(user.lower() == "delete"):
            word = input("Enter word to be deleted\n").capitalize()
            author = N_Psi_dict[word].author
            delete_word(N_Psi_dict, word)
            del authors[author]
            os.system('cls||clear')
            save_state = 0

        elif(user.lower() == "redefine"):
            word = input("Enter word to be redefined\n").capitalize()
            redefine_word(N_Psi_dict, authors, word)
            os.system('cls||clear')
            save_state = 0

        elif(user.lower() == "search"):
            while(True):
                option = input("Search by (word/subset/sequence/back)?\n").lower()
                if(option == "word"):
                    os.system('cls||clear')
                    sort_dict(N_Psi_dict)
                    search_for_word(N_Psi_dict, input("Enter word to search\n").lower().capitalize())
                    break
                elif(option == "subset"):
                    os.system('cls||clear')
                    sort_dict(N_Psi_dict)
                    search_for_subset(N_Psi_dict, input("Enter word to search\n").lower().capitalize(), int(input("Enter range of search\n")))
                    input()
                    break
                elif(option == "sequence"):
                    os.system('cls||clear')
                    search_for_sequence(N_Psi_dict, input("Enter sequence to search\n"))
                    input()
                    break
                elif(option == "back"):
                    os.system('cls||clear')
                    break
        
        elif(user.lower() == "size"):
            os.system('cls||clear')
            print("The size of the dictionary is: " + str(len(N_Psi_dict)) + "\n")
            input()

        elif(user.lower() == "load"):
            clarification = input("Would you like to override your current work? (Y/N)\n")
            while(True):
                if(clarification.upper() == "Y"):
                    load_save(N_Psi_dict)
                    os.system('cls||clear')
                    break
                elif(clarification.upper() == "N"):
                    os.system('cls||clear')
                    break
                else:
                    clarification = input("Please answer (Y/N).\n")

        elif(user.lower() == "save"):
            clarification = input("Would you like to overwrite your previous save file? (Y/N)\n")
            while(True):
                if(clarification.upper() == "Y"):
                    print("Saving... Please do not turn off your device!\n")
                    save(N_Psi_dict)
                    save_state = 1
                    break
                elif(clarification.upper() == "N"):
                    os.system('cls||clear')
                    break
                else:
                    clarification = input("Please answer (Y/N).\n")
            while(save_state == 1):
                save_and_quit = input("Would you like to quit after saving? (Y/N)\n")
                if(save_and_quit.upper() == "Y"):
                    os.system('cls||clear')
                    sys.exit()
                elif(save_and_quit.upper() == "N"):
                    os.system('cls||clear')
                    break
                
        elif(user.lower() == "quit"):
            if(save_state == 1):
                clarification = input("Are you sure you want to quit? You saved your data. (Y/N)\n")
            elif(save_state == 0):
                clarification = input("Are you sure you want to quit without saving? (Y/N)\n")
            while(True):
                if(clarification.upper() == "Y"):
                    os.system('cls||clear')
                    sys.exit("Force Quit Program")
                elif(clarification.upper() == "N"):
                    os.system('cls||clear')
                    break
                else:
                    clarification = input("Please answer (Y/N)\n")