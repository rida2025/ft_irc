#include "Commands.hpp"

int QUIT_COMMAND(Client &client, std::vector<Channels> &channels){
    std::string buffer;
    std::string channel_to_leave;
    int flag = -1;
    for (size_t i = 0;i < client.inside_channel.size();i++){
        flag = doesChannelExist(channels, client.inside_channel[i]);
        remove_member(channels[flag], client.nickName);
        channels[flag].members_count--;
        if (is_admin(channels[flag], client.nickName)){
            remove_admin(channels[flag], client.nickName);
        }
        buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " QUIT :EOF from client\r\n";
        broad_cast(channels[flag], buffer, "", "");
    }
    for (size_t i = 0;i < client.inside_channel.size();i++){
        flag = doesChannelExist(channels, client.inside_channel[i]);
        if (channels[flag].members.size() == 0){
            channels.erase(channels.begin()+flag);
        }
    }
    client.inside_channel.clear();
    return 0;
}