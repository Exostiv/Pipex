/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicoue <tnicoue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 10:20:18 by tnicoue           #+#    #+#             */
/*   Updated: 2022/05/24 10:29:15 by tnicoue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

char	*joincmd(char *oldpath, char *cmd)
{
	char	*newpath;

	newpath = ft_strjoin(oldpath, cmd);
	free(oldpath);
	return (newpath);
}

char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*accessverif(char **argv, char **cmd)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		argv[i] = joincmd(ft_strjoin(argv[i], "/"), cmd[0]);
		if (access(argv[i], R_OK) == 0)
			return (argv[i]);
		i++;
	}
	return (NULL);
}

void	run(char *argv, char **envp)
{
	char	**cmd;
	char	**oldpath;
	char	*path;
	int		check;

	check = 0;
	oldpath = ft_split(get_path(envp), ':');
	cmd = ft_split(argv, ' ');
	path = accessverif(oldpath, cmd);
	free_tab(oldpath);
	if (execve(path, cmd, envp) == 0)
		check = 1;
	if (check == 1)
	{
		free(path);
		free_tab(cmd);
		perror("Error bad commands");
		exit(EXIT_FAILURE);
	}
}
