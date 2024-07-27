/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-jira <mel-jira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 07:32:15 by mel-jira          #+#    #+#             */
/*   Updated: 2024/07/26 12:50:11 by mel-jira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

int INVITE_COMMAND(int fd, std::vector<std::string> &cmds, std::map<int, Client> &mapo, std::vector<Channels> &channels){
    int flag = -1;
    std::string buffer;
    if (cmds.size() >= 3)
    {
        if (isHeInServer(mapo, cmds[1]))
        {
            if (check_channel(fd, channels, cmds[2], mapo[fd]) >= 0)
            {
                flag = check_channel(fd, channels, cmds[2], mapo[fd]);
                if (in_channel(channels[flag], cmds[1])){
                    buffer = ":ircserver 443 " + mapo[fd].nickName + " " + cmds[1] + " " + channels[flag].channel_name + " :is already on channel\r\n";
                    send(fd, buffer.c_str(), buffer.length(), 0);
                }
                else
                {
                    if (is_admin(channels[flag], mapo[fd].nickName)){
                        channels[flag].invite_list.push_back(cmds[1]);
                        buffer = ":ircserver 341 " + mapo[fd].nickName + " " + cmds[1] + " " + cmds[2] + "\r\n";
                        send(fd, buffer.c_str(), buffer.length(), 0);
                        buffer = ":" + mapo[fd].nickName + "!" + mapo[fd].userName + "@" + mapo[fd].ip + " INVITE " + cmds[1] + " " + cmds[2] + "\r\n";
                        send(get_fd(mapo, cmds[1]), buffer.c_str(), buffer.length(), 0);
                    }
                    else{
                        buffer = ":ircserver 482 " + mapo[fd].nickName + " " + channels[flag].channel_name + " :You're not channel operator\r\n";
                        send(fd, buffer.c_str(), buffer.length(), 0);
                    }
                }
            }
        }
        else{
            buffer = ":ircserver 401 " + mapo[fd].nickName  + " " + cmds[2] + " :No such nick/channel\r\n";
            send(fd, buffer.c_str(), buffer.length(), 0);
        }
    }
    else
    {
        buffer = ":ircserver 461 " + mapo[fd].nickName + " " + cmds[0] + " :Not enough parameters\r\n";
        send(fd, buffer.c_str(), buffer.length(), 0);
    }
    return 0;
}
