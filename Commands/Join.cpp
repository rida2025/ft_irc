#include "Commands.hpp"

int    JOIN_COMMAND(int fd, std::vector<std::string> &cmds, Client &client, std::vector<Channels> &channels)
{
    size_t p = 0;
    int flag = -1;
    p_c command = splite_str(cmds);
    std::string buffer;
    if (cmds.size() < 2)
    {
        buffer = ":ircserver 461 " + client.nickName + " " + cmds[0] + " :Not enough parameters\r\n";
        send(fd, buffer.c_str(), buffer.length(), 0);
        return 1;
    }
    for (size_t i = 0; i < command.channels_name.size();i++)
    {
        flag = -1;
        if ((command.channels_name[i][0] == '#' || command.channels_name[i][0] == '&') && command.channels_name[i].size() > 1 && !strchr(" \a\0\n\r,", command.channels_name[i][1]))
        {
            flag = doesChannelExist(channels, command.channels_name[i]);
            if (flag == -1)
                create_join_channel(channels, command, i, client);
            else {
                if (in_channel(channels[flag], client.nickName))
                    continue ;
                if (isHeInvited(channels[flag], client.nickName)){
                    join_channel(channels, flag, client);
                    continue ;
                }
                if (channels[flag].is_invite_only){
                    buffer = ":ircserver 473 " + client.nickName + " " + channels[flag].channel_name + " :Cannot join channel, you must be invited (+i)\r\n";
                    send(fd, buffer.c_str(), buffer.length(), 0);
                    continue ;
                }
                if (channels[flag].is_limit){
                    if (channels[flag].members_limit <= channels[flag].members.size()){
                        buffer = ":ircserver 471 " + client.nickName + " " + channels[flag].channel_name + " :Cannot join channel (+l)\r\n";
                        send(fd, buffer.c_str(), buffer.length(), 0);
                        continue ;
                    }
                    else{
                        if (channels[flag].is_key){
                            if (command.channels_key.size() >= p + 1 && channels[flag].channel_key == command.channels_key[p]){
                                p++;
                                join_channel(channels, flag, client);
                            }
                            else{
                                p++;
                                buffer = ":ircserver 475 " + client.nickName + " " + channels[flag].channel_name + " :Cannot join channel, you need the correct key (+k)\r\n";
                                send(fd, buffer.c_str(), buffer.length(), 0);
                            }
                        }
                        else{
                            join_channel(channels, flag, client);
                        }
                    }
                }
                else if (channels[flag].is_key){
                    if (command.channels_key.size() >= (p + 1) && channels[flag].channel_key == command.channels_key[p]){
                        p++;
                        join_channel(channels, flag, client);
                    }
                    else{
                        p++;
                        buffer = ":ircserver 475 " + client.nickName + " " + channels[flag].channel_name + " :Cannot join channel, you need the correct key (+k)\r\n";
                        send(fd, buffer.c_str(), buffer.length(), 0);
                    }
                }
                else{
                    join_channel(channels, flag, client);
                }
            }
        }
        else{
            buffer = ":ircserver 403 " + client.nickName + " " + command.channels_name[i] + " :No such channel\r\n";
            send(fd, buffer.c_str(), buffer.length(), 0);
        }
    }
    return 0;
}