/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-jira <mel-jira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 07:30:58 by mel-jira          #+#    #+#             */
/*   Updated: 2024/07/23 18:55:35 by mel-jira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

int     MODE_COMMAND(int fd, std::vector<std::string> &cmds, Client &client, std::vector<Channels> &channels){
    size_t arg = 0;
    char sign = '+';
    std::string error = "i";
    int flag = -1;
    std::string buffer1 = "";
    std::string buffer = "";
    std::vector<std::pair<std::string, std::string> > changes;
    int key = 3;
    int x = 0;
    if (cmds.size() >= 2)
    {
        for (size_t j = 0; j < channels.size();j++) 
        {
            if (cmds[1] == channels[j].channel_name){
                flag = j;
                break ;
            }
        }
        if (flag != -1) 
        {
            if (!in_channel(channels[flag], client.nickName)){
                buffer = ":ircserver 442 " + client.nickName + " " + cmds[1] + " :You're not on that channel\r\n";
                send(fd, buffer.c_str(), buffer.length(), 0);
                return 0;
            }
            if (cmds.size() == 2){
                buffer = ":ircserver 324 " + client.nickName + " " + cmds[1] + " " + getChannelModes(channels[flag]) + "\r\n";
                send(fd, buffer.c_str(), buffer.length(), 0);
                buffer = ":ircserver 329 " + client.nickName + " " + cmds[1] + " " + longlongToString(channels[flag].channel_create_time) + "\r\n";
                send(fd, buffer.c_str(), buffer.length(), 0);
                return 0;
            }
            if (cmds.size() >= 3) 
            {
                if (!is_admin(channels[flag], client.nickName)){
                    buffer = ":ircserver 482 " + client.nickName + " " + cmds[1] + " :You're not channel operator\r\n";
                    send(fd, buffer.c_str(), buffer.length(), 0);
                    return 0;
                }
                for (int i = 0;cmds[2][i];i++){ 
                    while (cmds[2][i] && (cmds[2][i] == '+' || cmds[2][i] == '-')){
                        sign = cmds[2][i];
                        i++;
                    }
                    if (sign == '+' && cmds[2][i])
                        buffer1 += sign;
                    else if (sign == '-' && cmds[2][i])
                        buffer1 += sign;
                    if (strchr("lkito", cmds[2][i]))
                        buffer1 += cmds[2][i];
                    else {
                        error = cmds[2][i];
                        buffer1 = ":ircserver 472 " + client.nickName + " " + error + " :is an unknown mode char to me\r\n";
                        send(fd, buffer1.c_str(), buffer1.length(), 0);
                        return 0;
                    }
                }
                for (int i = 0; buffer1[i]; i += 2){
                    if (buffer1[i] && buffer1[i+1] && buffer1[i] == '+' && strchr("kl", buffer1[i+1])){
                        arg++;
                    }
                    else if (buffer1[i] && buffer1[i+1] && buffer1[i] == '-' && strchr("k", buffer1[i+1])){
                        arg++;
                    }
                }
                if (arg > cmds.size() - 3){
                    buffer = ":ircserver 461 " + client.nickName + " " + cmds[0] + " :Not enough parameters\r\n";
                    send(fd, buffer.c_str(), buffer.length(), 0);
                    return 0;
                }
                for (size_t i = 0;i < buffer1.length();i +=2)
                {
                    if (!strncmp(&buffer1[i], "+i", 2)){
                        if (channels[flag].is_invite_only){
                        }
                        else if (!channels[flag].is_invite_only){
                            lookfor("-i", changes);
                            changes.push_back(std::make_pair("+i", ""));
                            channels[flag].is_invite_only = true;
                        }
                    }
                    else if (!strncmp(&buffer1[i], "-i", 2)){
                        if (channels[flag].is_invite_only){
                            lookfor("+i", changes);
                            changes.push_back(std::make_pair("-i", ""));
                            channels[flag].is_invite_only = false;
                        }
                        else if (!channels[flag].is_invite_only){
                        }
                    }
                    else if (!strncmp(&buffer1[i], "+t", 2)){
                        if (channels[flag].is_topic){
                        }
                        else if (!channels[flag].is_topic){
                            lookfor("-t", changes);
                            changes.push_back(std::make_pair("+t", ""));
                            channels[flag].is_topic = true;
                        }
                    }
                    else if (!strncmp(&buffer1[i], "-t", 2)){
                        if (channels[flag].is_topic){
                            lookfor("+t", changes);
                            changes.push_back(std::make_pair("-t", ""));
                            channels[flag].is_topic = false;
                            channels[flag].channel_topic = "";
                        }
                        else if (!channels[flag].is_topic){
                        }
                    }
                    else if (!strncmp(&buffer1[i], "+k", 2)){
                        if (channels[flag].is_key){
                            buffer = ":ircserver 467 " + client.nickName + " " + channels[flag].channel_name + " :Channel key already set\r\n";
                            send(fd, buffer.c_str(), buffer.length(), 0);
                        }
                        else if (!channels[flag].is_key){
                            lookfor("-k", changes);
                            changes.push_back(std::make_pair("+k", cmds[key]));
                            channels[flag].channel_key = cmds[key++];
                            channels[flag].is_key = true;
                        }
                    }
                    else if (!strncmp(&buffer1[i], "-k", 2)){
                        if (channels[flag].is_key){
                            if (channels[flag].channel_key == cmds[key]){
                                lookfor("+k", changes);
                                changes.push_back(std::make_pair("-k", ""));
                                channels[flag].is_key = false;
                                channels[flag].channel_key = "";
                                key++;
                            }
                            else{
                                buffer = ":ircserver 467 " + client.nickName + " " + channels[flag].channel_name + " :Channel key already set\r\n";
                                send(fd, buffer.c_str(), buffer.length(), 0);
                            }
                        }
                        else if (!channels[flag].is_key){
                            buffer = ":ircserver 467 " + client.nickName + " " + channels[flag].channel_name + " :Channel key already set\r\n";
                            send(fd, buffer.c_str(), buffer.length(), 0);
                        }
                    }
                    else if (!strncmp(&buffer1[i], "+l", 2)){
                        if (channels[flag].is_limit){
                            if ((int)channels[flag].members_limit != atoi(cmds[key].c_str()) && atoi(cmds[key].c_str()) > 0){
                                lookfor("+l", changes);
                                changes.push_back(std::make_pair("+l", cmds[key].c_str()));
                                channels[flag].members_limit = atoi(cmds[key].c_str());
                                key++;
                            }
                        }
                        else if (!channels[flag].is_limit){
                            lookfor("-l", changes);
                            changes.push_back(std::make_pair("+l", cmds[key].c_str()));
                            channels[flag].members_limit = atoi(cmds[key].c_str());
                            channels[flag].is_limit = true;
                            key++;
                        }
                    }
                    else if (!strncmp(&buffer1[i], "-l", 2)){
                        if (channels[flag].is_limit){
                            lookfor("+l", changes);
                            changes.push_back(std::make_pair("-l", ""));
                            channels[flag].is_limit = false;
                        }
                    }
                    else if (!strncmp(&buffer1[i], "+o", 2)){
                        if (in_channel(channels[flag], cmds[key]))
                        {   
                            if (already_op(channels[flag], cmds[key])){
                                key++;
                            }
                            else{
                                changes.push_back(std::make_pair("+o", cmds[key]));
                                channels[flag].admin_list.push_back(cmds[key++]);
                            }
                        }
                        else{
                            buffer = ":ircserver 401 " + client.nickName + " " + cmds[key] + " :No such nick/channel\r\n";
                            key++;
                            send(fd, buffer.c_str(), buffer.length(), 0);
                        }
                    }
                    else if (!strncmp(&buffer1[i], "-o", 2)){
                        if (in_channel(channels[flag], cmds[key])){
                            if (!already_op(channels[flag], cmds[key])){
                                key++;
                            }
                            else if (already_op(channels[flag], cmds[key])){
                                changes.push_back(std::make_pair("-o", cmds[key]));
                                remove_admin(channels[flag], cmds[key]);
                                key++;
                            }
                        }
                        else{
                            buffer = ":ircserver 401 " + client.nickName + " " + cmds[key] + " :No such nick/channel\r\n";
                            key++;
                            send(fd, buffer.c_str(), buffer.length(), 0);
                        }
                    }
                }
                buffer1.clear();
                for (size_t i = 0;i < changes.size();i++){
                    buffer1 += changes[i].first;
                    if (!changes[i].second.empty())
                        x++;
                }
                if (x)
                    buffer1 += " ";
                for (size_t i = 0;i < changes.size();i++){
                    buffer1 += changes[i].second;
                    if (i+1 < changes.size())
                        buffer1 += " ";
                }
                buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " MODE " + channels[flag].channel_name + " " + buffer1 + "\r\n";
                if (!changes.empty())
                    broad_cast(channels[flag], buffer, "", "");
            }
            else{
                buffer = ":ircserver 461 " + client.nickName + " " + cmds[0] + " :Not enough parameters\r\n";
                send(fd, buffer.c_str(), buffer.length(), 0);
                return 0;
            }
        }
        else{
            buffer = ":ircserver 403 " + cmds[1] + " :No such channel\r\n";
            send(fd, buffer.c_str(), buffer.length(), 0);
            return 0;
        }
    }
    else{
        buffer = ":ircserver 461 " + client.nickName + " " + cmds[0] + " :Not enough parameters\r\n";
        send(fd, buffer.c_str(), buffer.length(), 0);
    }
    return (0);
}
