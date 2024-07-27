#include "Commands.hpp"

int KICK_COMMAND(int fd, std::vector<std::string> &cmds, Client &client, std::vector<Channels> &channels)
{
    int kicked = 0;
    int flag = -1;
    std::string buffer;
    std::string reason;
    if (cmds.size() < 3 || (cmds.size() == 3 && cmds[2] == ":"))
    {
        buffer = ":ircserver 461 " + client.nickName + " " + cmds[0] + " :Not enough parameters\r\n";
        send(fd, buffer.c_str(), buffer.length(), 0);
        return 1;
    }
    if (check_channel(fd, channels, cmds[1], client) == -1)
        return 1;
    flag = check_channel(fd, channels, cmds[1], client);
    if (!is_admin(channels[flag], client.nickName)){
        buffer = ":ircserver 482 " + client.nickName + " " + channels[flag].channel_name + " :You're not channel operator\r\n";
        send(fd, buffer.c_str(), buffer.length(), 0);
        return 1;
    }
    if (in_channel(channels[flag], cmds[2])){
        if (cmds.size() == 3){
            buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " KICK " + cmds[1] + " " + cmds[2] + " :" + cmds[2] + "\r\n";
        }
        else if (cmds.size() >= 4 && cmds[3][0] == ':'){
            for (size_t i = 3; i < cmds.size();i++){
                if (i == 3)
                    reason += removeComma(cmds[i]);
                else
                {
                    reason += cmds[i];
                    if (i+1 < cmds.size())
                        reason += " ";
                }
            }
            buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " KICK " + cmds[1] + " " + cmds[2] + " :" + reason + "\r\n";
        }
        else if (cmds.size() >= 4 && cmds[3][0] != ':')
            buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " KICK " + cmds[1] + " " + cmds[2] + " :" + cmds[2] + "\r\n";
        broad_cast(channels[flag], buffer, "", "");
        remove_member(channels[flag], cmds[2]);
        channels[flag].members_count--;
        kicked = 1;
    }
    if (is_admin(channels[flag], cmds[2])){
        for (size_t j = 0;j < channels[flag].admin_list.size();j++){
            if (cmds[2] == channels[flag].admin_list[j]){
                channels[flag].admin_list.erase(channels[flag].admin_list.begin()+j);
                kicked = 1;
                break ;
            }
        }
    }
    if (kicked){
        for (std::vector<std::string>::iterator it = client.inside_channel.begin();it != client.inside_channel.end();it++){
            if (*it == channels[flag].channel_name){
                client.inside_channel.erase(it);
                break ;
            }
        }
    }
    if (!kicked){
        buffer = ":ircserver 441 " + cmds[2] + " " + channels[flag].channel_name + " :They aren't on that channel\r\n";
        send(fd, buffer.c_str(), buffer.length(), 0);
    }
    return 0;
}