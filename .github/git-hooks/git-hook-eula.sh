#!/bin/bash

# Ansi color codes
BLACK='\033[0;30m'
GREY='\033[1;30m'
RED='\033[0;31m'
LIGHT_RED='\033[1;31m'
GREEN='\033[0;32m'
LIGHT_GREEN='\033[1;32m'
ORANGE='\033[0;33m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
LIGHT_BLUE='\033[1;34m'
PURPLE='\033[0;35m'
LIGHT_PURPLE='\033[1;35m'
CYAN='\033[0;36m'
LIGHT_CYAN='\033[1;36m'
LIGHT_GRAY='\033[0;37m'
WHITE='\033[1;37m'
RESET_COLOR='\033[0m'

# Set defualt input to keyboard so that git hook can ask for input
exec < /dev/tty

# Check if eula file exist:
if [ ! -f ./.github/eula ]
then
    # Show why user needs to accept
    printf '%b\n' \
            "" \
            "Hi, welcome to ${LIGHT_GREEN}Hampus Toft${RESET_COLOR}'s ${GREY}(hampus.toft.1883@student.uu.se)${RESET_COLOR} IOOPM respoitory." \
            "We use ${LIGHT_RED}strict guidlines${RESET_COLOR} in how to contribute to this project to keep our history maintainable and clean" \
            "To enforce these guidlines we need to ask for your premission to execute scripts contained in the ${YELLOW}'.github/git-hooks'${RESET_COLOR} folder" \
            "These scripts do a copule diffrent things to make your contribution align with our standards" \
            "Here are some examples:" \
            "${YELLOW}  1. Read commit message and block messages that have a invalid subject line${RESET_COLOR}" \
            "${YELLOW}  2. Read git branch name to make sure it conforms with guidlines${RESET_COLOR}" \
            "${YELLOW}  3. Block commits and pushes against protected branches that should instead be made withh GitHub Pull Request${RESET_COLOR}" \
            "${YELLOW}  4. Block work-in-progress commits from entering remote repository, and gives suggestions on how to remove them${RESET_COLOR}" \
            "We Strongly recommend that you do not accept this agreement untill" \
            "you have sufficient knowladge of how they inforce our guidlines" \
            ""

    # Prompt user for answer
    printf '%b' "Do you accept git-hook executions to be able to contribute to this repository? [${LIGHT_GREEN}y${RESET_COLOR}/${LIGHT_RED}n${RESET_COLOR}]: "
    read answer
    
    # Check answer
    if [[ $answer = y ]] || [[ $answer = Y ]]
    then
        # Inform user that they have agreed
        printf '%b\n' \
                "" \
                "You have accepted that all git hooks can run on your system." \
                "A file has been created at ./.github/eula to save your answer" \
                "To remove access to our hooks you need to delete the file ./.github/eula" \
                ""

        # Create file with text
        printf '%s\n' \
                "# You have accepted that all git hooks can run on your system." \
                "# To remove this premission simply delete this file." \
                "# or change to false" \
                "eula: true" \
                > ./.github/eula
    else
        # Inform user that they can not contribute to project
        # if they do not agree
        printf '%b\n' \
                "" \
                "${RED}You did not accept our agreement policy${RESET_COLOR}" \
                "${RED}Telling git to forceliby reject commits and or pushes${RESET_COLOR}" \
                ""
        
        # Exit wíth 1 which will cause any git-hook running this script to exit with 1 as well
        exit 1
    fi
else
    # Eula file exist Check if it is accepted using grep
    answer=$(grep -Po 'eula: \K.*' ./.github/eula)
    if [[ $answer != true ]]
    then
        # User has changed eula to false
        printf '%b\n' \
                "" \
                "${RED}EULA is set to false${RESET_COLOR}" \
                "You have not accepted our agreement policy" \
                "To ${GREEN}accept${RESET_COLOR} our agreement policy, please change eula to ${LIGHT_GREEN}true${RESET_COLOR} inside of ${YELLOW}'.github/eula'${RESET_COLOR}" \
                "${RED}Telling git to forceliby reject commits and or pushes${RESET_COLOR}" \
                ""

        # Exit wíth 1 which will cause any git-hook running this script to exit with 1 as well
        exit 1
    fi
fi