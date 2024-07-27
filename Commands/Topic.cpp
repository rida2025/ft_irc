#include "Commands.hpp"

int TOPIC_COMMAND(int fd, std::vector<std::string> &cmds, Client &client, std::vector<Channels> &channels){
    int flag = -1;
    std::string buffer;
    std::string topic;
    if (cmds.size() < 2)
    {
        buffer = ":ircserver 461 " + cmds[0] + " :Not enough parameters\r\n";
        send(fd, buffer.c_str(), buffer.length(), 0);
        return 1;
    }
    if (check_channel(fd, channels, cmds[1], client) >= 0)
    {
        flag = check_channel(fd, channels, cmds[1], client);
        if (!in_channel(channels[flag], client.nickName)){
            buffer = ":ircserver 442 " + client.nickName + " " + channels[flag].channel_name + " :You're not on that channel\r\n";
            send(fd, buffer.c_str(), buffer.length(), 0);
            return 0;
        }
        if (cmds.size() == 2){
            if (channels[flag].topic){
                buffer = ":ircserver 332 " + client.nickName + " " + channels[flag].channel_name + " :" + channels[flag].channel_topic + " \r\n";
                send(fd, buffer.c_str(), buffer.length(), 0);
                buffer = ":ircserver 333 " + client.nickName + " " + channels[flag].channel_name + " " + channels[flag].who_set_topic + " " + myto_string(channels[flag].topic_time) + "\r\n";
                send(fd, buffer.c_str(), buffer.length(), 0);
            }
            else{
                buffer = ":ircserver 331 " + client.nickName + " " + channels[flag].channel_name + " :" + channels[flag].channel_topic + "\r\n";
                send(fd, buffer.c_str(), buffer.length(), 0);
            }
            return 0;
        }
        if (channels[flag].is_topic){
            if (is_admin(channels[flag], client.nickName)){
                if(cmds.size() >= 3){
                    if (cmds.size() >= 3 && cmds[2][0] == ':'){
                        for (size_t i = 2;i < cmds.size();i++){
                            if (i == 2 && cmds[i] == ":")
                            {
                                topic += cmds[++i];
                            }
                            else if (i == 2 && cmds[i][0] == ':')
                            {
                                topic += removeComma(cmds[2]);
                            }
                            else
                                topic += cmds[i];
                            if (i+1 < cmds.size())
                                topic += " ";
                        }
                    }
                    else
                        topic = cmds[2];
                    channels[flag].who_set_topic = client.nickName;
                    channels[flag].topic_time = time(0);
                    channels[flag].channel_topic = topic;
                    buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " TOPIC " + channels[flag].channel_name + " :" + topic + "\r\n";
                    broad_cast(channels[flag], buffer, "", "");
                    channels[flag].topic = true;
                }
            }
            else{
                buffer = ":ircserver 482 " + client.nickName + " " + cmds[1] + " :You're not channel operator\r\n";
                send(fd, buffer.c_str(), buffer.length(), 0);
            }
        }
        else{
            if(cmds.size() >= 3){
                if (cmds.size() >= 3 && cmds[2][0] == ':'){
                    for (size_t i = 2;i < cmds.size();i++){
                        if (i == 2 && cmds[i] == ":")
                        {
                            topic += cmds[++i];
                        }
                        else if (i == 2 && cmds[i][0] == ':')
                        {
                            topic += removeComma(cmds[2]);
                        }
                        else
                            topic += cmds[i];
                        if (i+1 < cmds.size())
                            topic += " ";
                    }
                }
                else
                    topic = cmds[2];
                channels[flag].who_set_topic = client.nickName;
                channels[flag].topic_time = time(0);
                channels[flag].channel_topic = topic;
                buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " TOPIC " + channels[flag].channel_name + " :" + topic + "\r\n";
                broad_cast(channels[flag], buffer, "", "");
                channels[flag].topic = true;
            }
        }
    }
    return 0;
}