/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-jira <mel-jira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:25:53 by mel-jira          #+#    #+#             */
/*   Updated: 2024/07/22 19:02:06 by mel-jira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <cstring>
# include <algorithm>
# include <string>
#include <ctime>
#include <sys/socket.h>
# include "../server/Client.hpp"

typedef struct parsing_command
{
    std::string command;
    std::vector<std::string> channels_name;
    std::vector<std::string> channels_key;
}   p_c;

class Channels
{
    public:
        std::string                 channel_name;
        bool                        is_invite_only;         //invite
        std::vector<std::string>    invite_list;
        bool                        is_limit;               //limit
        size_t                      members_limit;
        size_t                      members_count;
        bool                        is_topic;               //topic
        std::string                 channel_topic;
        bool                        topic;
        long long                   topic_time;
        std::string                 who_set_topic;
        bool                        is_key;                 //key
        std::string                 channel_key;
        std::vector<std::string>    admin_list;
        std::map<std::string, int>  members;
        long long                   channel_create_time;
        Channels();
};

//important commands

void IS_COMMAND_VALID(int fd, std::string &str, std::map<int, Client> &mapo, std::vector<Channels> &channels);
int    JOIN_COMMAND(int fd, std::vector<std::string> &cmds, Client &client, std::vector<Channels> &channels);
int TOPIC_COMMAND(int fd, std::vector<std::string> &cmds, Client &client, std::vector<Channels> &channels);
int KICK_COMMAND(int fd, std::vector<std::string> &cmds, Client &client, std::vector<Channels> &channels);
int INVITE_COMMAND(int fd, std::vector<std::string> &cmds, std::map<int, Client> &mapo, std::vector<Channels> &channels);
int     MODE_COMMAND(int fd, std::vector<std::string> &cmds, Client &client, std::vector<Channels> &channels);
int PRIVMSG_COMMAND(int fd, std::vector<std::string> &cmds, std::map<int, Client> &mapo, std::vector<Channels> &channels);
void    HAKIM_COMMAND(int fd);
int QUIT_COMMAND(Client &client, std::vector<Channels> &channels);

//helper functions
void    join_channel(std::vector<Channels> &channels, int index, Client &client);
void    create_join_channel(std::vector<Channels> &channels, p_c &command, int index, Client &client);
std::string getChannelModes(Channels &channel);
std::string sizetToString(size_t value);
std::string longlongToString(long long value);
bool isHeInvited(Channels &channel, std::string &nickname);
bool isHeInServer(std::map<int, Client> &map, std::string nickname);
bool imInChannel(Channels &channel, std::string &name);
void    broad_cast(Channels &channel, std::string error, std::string mid, std::string message);
int check_channel(int fd, std::vector<Channels> &channels, std::string &channel_name, Client &client);
int doesChannelExist(std::vector<Channels> &channels, std::string &channel_name);
int get_fd(std::map<int ,Client> &map, std::string &nickname);
void    remove_member(Channels &channel, std::string &name);
bool    is_admin(Channels &channel, std::string &name);
void    remove_admin(Channels &channel, std::string &name);
bool    in_channel(Channels &channel, std::string &name);
bool already_op(Channels &channel, std::string &name);
void    lookfor(std::string look, std::vector<std::pair<std::string, std::string> > &changes);
p_c splite_str(std::vector<std::string> &str);
p_c splite_message(std::vector<std::string> &str);
std::vector<std::string> split(std::string &str, char sep);
std::string myto_string(long long value);
std::vector<std::string> spliteCommand(std::string &str);
std::string removeComma(std::string &str);
