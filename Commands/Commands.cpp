#include "Commands.hpp"
Channels::Channels()
{
    channel_name = "";
    channel_topic = "No topic is set.";
    channel_key = "";
    is_invite_only = false;
    is_limit = false;
    is_key = false;
    is_topic = false;
    topic = false;
    members_limit = 0;
    members_count = 0;
    topic_time = 0;
}

std::vector<std::string> spliteCommand(std::string &str){
    std::vector<std::string> names;
    std::stringstream ss(str);
    std::string name = "";
    
    while (ss >> name) {
        names.push_back(name);
        name.clear();
    }
    return (names);
}

std::string myto_string(long long value){
    std::stringstream oss;
    oss << value;
    return oss.str();
}

std::vector<std::string> split(std::string &str, char sep){
    std::vector<std::string> names;
    std::stringstream ss(str);
    std::string name;
    
    while (std::getline(ss, name, sep)) {
        names.push_back(name);
    }
    return (names);
}

p_c splite_str(std::vector<std::string> &str)
{
    p_c command;

    command.command = str[0];
    if (str.size() > 1){
        command.channels_name = split(str[1], ',');
    }
    if (str.size() > 2)
        command.channels_key = split(str[2], ',');
    return (command);
}

std::string removeComma(std::string &str){
    std::string tmp = "";
    for (size_t i = 0; i < str.length();i++){
        if (i != 0)
            tmp += str[i];
    }
    return tmp;
}

p_c splite_message(std::vector<std::string> &str){
    p_c command;
    std::string buffer;

    command.command = str[0];
    if (str.size() > 1){
        command.channels_name = split(str[1], ',');
    }
    if (str.size() > 2){
        if (str[2][0] != ':'){
            command.channels_key.push_back(str[2]);
        }
        else
        {
            for (size_t i = 2; i < str.size();i++){
                if (i == 2 && str[i][0] == ':')
                {
                    buffer += removeComma(str[2]);
                }
                else if (i == 2 && str[i] == ":" && str.size() >= 4){
                    buffer += str[++i];
                }
                else{
                    buffer += str[i];
                }
                if (i+1 < str.size())
                    buffer += " ";
            }
            command.channels_key.push_back(buffer);
        }
    }
    return (command);
}

void    lookfor(std::string look, std::vector<std::pair<std::string, std::string> > &changes){
    for (size_t i = 0;i < changes.size();i++)
    {
        if (changes[i].first == look)
        {
            changes.erase(changes.begin()+i);
            break ;
        }
    }
}

bool already_op(Channels &channel, std::string &name){
    for (size_t i = 0;i < channel.admin_list.size();i++){
        if (channel.admin_list[i] == name)
            return (true);
    }
    return (false);
}

bool    in_channel(Channels &channel, std::string &name){
    std::string tmp = "@" + name;
    for (std::map<std::string, int>::iterator it = channel.members.begin();it != channel.members.end();it++){
        if (it->first == name || it->first == tmp)
            return (true);
    }
    return (false);
}

void    remove_admin(Channels &channel, std::string &name){
    for (size_t i = 0;i < channel.admin_list.size();i++){
        if (channel.admin_list[i] == name)
        {
            channel.admin_list.erase(channel.admin_list.begin()+i);
            break ;
        }
    }
}

bool    is_admin(Channels &channel, std::string &name){
    for (size_t i = 0;i < channel.admin_list.size();i++){
        if (channel.admin_list[i] == name)
            return (true);
    }
    return (false);
}

void    remove_member(Channels &channel, std::string &name){
    for (std::map<std::string, int>::iterator it =  channel.members.begin();it != channel.members.end();it++){
        if (it->first == name || it->first == ("@" + name))
        {
            channel.members.erase(it);
            break ;
        }
    }
}

int get_fd(std::map<int ,Client> &map, std::string &nickname){
    for (size_t i = 0;i < map.size();i++){
        if (map[i].nickName == nickname){
            return (map[i].fd);
        }
    }
    return (-1);
}

int doesChannelExist(std::vector<Channels> &channels, std::string &channel_name){
    int flag = -1;
    for (size_t i = 0;i < channels.size();i++)
    {
        if (channel_name == channels[i].channel_name)
        {
            flag = i;
            break ;
        }
    }
    return (flag);
}

int check_channel(int fd, std::vector<Channels> &channels, std::string &channel_name, Client &client){
    int flag = -1;
    std::string buffer;
    for (size_t i = 0;i < channels.size();i++)
    {
        if (channel_name == channels[i].channel_name)
        {
            flag = i;
            break ;
        }
    }
    if (flag == -1)
    {
        buffer = ":ircserver 403 " + client.nickName + " " + channel_name + " :No such channel\r\n";
        send(fd, buffer.c_str(), buffer.length(), 0);
    }
    return (flag);
}

void    broad_cast(Channels &channel, std::string error, std::string mid, std::string message){
    std::string buffer;
    for (std::map<std::string, int>::iterator it =  channel.members.begin();it != channel.members.end();it++){
        buffer = error + mid + message;
        send(it->second, buffer.c_str(), buffer.length(), 0);
    }
}

bool imInChannel(Channels &channel, std::string &name){
    std::string admin = "@" + name;
    for (size_t i = 0;i < channel.admin_list.size();i++){
        if (name == channel.admin_list[i]){
            return (true);
        }
    }
    if (channel.members.find(name) != channel.members.end() || channel.members.find(admin) != channel.members.end()){
        return (true);
    }
    return (false);
}

bool isHeInServer(std::map<int, Client> &map, std::string nickname){
    for (size_t i = 0; i < map.size();i++){
        if (map[i].nickName == nickname)
            return (true);
    }
    return (false);
}

bool isHeInvited(Channels &channel, std::string &nickname){
    for (size_t i = 0;i < channel.invite_list.size();i++){
        if (channel.invite_list[i] == nickname)
            return (true);
    }
    return (false);
}

std::string longlongToString(long long value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string sizetToString(size_t value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string getChannelModes(Channels &channel){
    std::string buffer1 = "";
    std::string buffer2 = "";
    bool flag = false;
    if (channel.is_key){
        buffer1 += "+k";
        buffer2 += channel.channel_key;
        flag = true;
    }
    if (channel.is_topic){
        buffer1 += "+t";
    }
    if (channel.is_limit){
        buffer1 += "+l";
        if (flag)
            buffer2 += " ";
        buffer2 += sizetToString(channel.members_limit);
    }
    if (channel.is_invite_only){
        buffer1 += "+i";
    }
    buffer1 = buffer1 + " " + buffer2;
    return (buffer1);
}

void    create_join_channel(std::vector<Channels> &channels, p_c &command, int index, Client &client){
    Channels channel;
    std::string buffer;
    channel.members_count = 1;
    channel.channel_name = command.channels_name[index];
    channel.channel_create_time = time(0);
    channel.members.insert(std::make_pair("@" + client.nickName, client.fd));
    channel.admin_list.push_back(client.nickName);
    channel.channel_topic = "No topic is set.";
    client.inside_channel.push_back(command.channels_name[index]);
    channels.push_back(channel);
    buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " JOIN " + command.channels_name[index] + "\r\n";
    send(client.fd, buffer.c_str(), buffer.length(), 0);
    buffer = ":ircserver 353 " + client.nickName + " = " + command.channels_name[index] + " :@" + client.nickName + "\r\n";
    send(client.fd, buffer.c_str(), buffer.length(), 0);
    buffer = ":ircserver 366 " + client.nickName + " " + command.channels_name[index] + " :End of /NAMES list.\r\n";
    send(client.fd, buffer.c_str(), buffer.length(), 0);
}

void    join_channel(std::vector<Channels> &channels, int index, Client &client)
{
    channels[index].members_count++;
    std::string buffer;
    channels[index].members.insert(std::make_pair(client.nickName, client.fd));
    client.inside_channel.push_back(channels[index].channel_name);
    for (size_t i = 0;i < channels[index].invite_list.size();i++)
    {
        if (channels[index].invite_list[i] == client.nickName)
        {
            channels[index].invite_list[i].erase();
            break ;
        }
    }
    for (std::map<std::string, int>::iterator it = channels[index].members.begin();it != channels[index].members.end();it++)
    {
        buffer = ":" + client.nickName + "!" + client.userName + "@" + client.ip + " JOIN " + channels[index].channel_name + "\r\n";
        send(it->second, buffer.c_str(), buffer.length(), 0);
    }
    if (channels[index].topic){
        buffer = ":ircserver 332 " + client.nickName + " " + channels[index].channel_name + " :" + channels[index].channel_topic + "\r\n";
        send(client.fd, buffer.c_str(), buffer.length(), 0);
        buffer = ":ircserver 333 " + client.nickName + " " + channels[index].channel_name + " " + channels[index].who_set_topic + " " + myto_string(channels[index].topic_time) + "\r\n";
        send(client.fd, buffer.c_str(), buffer.length(), 0);
    }
    buffer = ":ircserver 353 " + client.nickName + " = " + channels[index].channel_name + " :";
    for (std::map<std::string, int>::iterator ito = channels[index].members.begin(); ito != channels[index].members.end(); ito++)
    {
        buffer += ito->first;
        std::map<std::string, int>::iterator nextIto = ito;
        ++nextIto;
        if (nextIto != channels[index].members.end()) {
            buffer += " ";
        }
    }
    buffer += "\r\n";
    send(client.fd, buffer.c_str(), buffer.length(), 0);
    buffer = ":ircserver 366 " + client.nickName + " " + channels[index].channel_name + " :End of /NAMES list.\r\n";
    send(client.fd, buffer.c_str(), buffer.length(), 0);
}

void IS_COMMAND_VALID(int fd, std::string &str, std::map<int, Client> &mapo, std::vector<Channels> &channels)
{
    std::vector<std::string> cmds;
    cmds = spliteCommand(str);
    std::string cmd = cmds[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
    std::string buffer;
    if (cmds.size() >= 1)
    {
        if (cmd == "JOIN")
        {
            JOIN_COMMAND(fd, cmds, mapo[fd], channels);
        }
        else if (cmd == "MODE"){
            MODE_COMMAND(fd, cmds, mapo[fd], channels);
        }
        else if (cmd == "KICK"){
            KICK_COMMAND(fd, cmds, mapo[fd], channels);
        }
        else if (cmd == "INVITE"){
            INVITE_COMMAND(fd, cmds, mapo, channels);
        }
        else if (cmd == "TOPIC"){
            TOPIC_COMMAND(fd, cmds, mapo[fd], channels);
        }
        else if (cmd == "PRIVMSG"){
            PRIVMSG_COMMAND(fd, cmds, mapo, channels);
        }
        else if (cmd == "HAKIM"){
            HAKIM_COMMAND(fd);
        }
        else
        {
            buffer = ":ircserver 421 " + mapo[fd].nickName + " " + cmds[0] + " :Unknown command\r\n";
            send(fd, buffer.c_str(), buffer.length(), 0);
        }
    }
}