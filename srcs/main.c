/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicoue <tnicoue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 11:46:13 by tnicoue           #+#    #+#             */
/*   Updated: 2022/03/22 09:31:02 by tnicoue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"


void	parent_process(int fileout, char **argv, char **envp, int *end)
{
	dup2(end[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(end[1]);
	run(argv[3], envp);
}

void	child_process(int filein, char **argv, char **envp, int *end)
{
	dup2(end[1], STDOUT_FILENO);
	dup2(filein, STDIN_FILENO);
	close(end[0]);
	run(argv[2], envp);
}

void	pipex(int filein, int fileout, char **argv, char **envp)
{
	int		end[2];
	int		parent;

	if (pipe(end) == -1)
	{
		perror("Pipe failed");
		exit(EXIT_FAILURE);
	}
	parent = fork();
	if (parent == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (parent == 0)
		child_process(filein, argv, envp, end);
	else
	{
		waitpid(parent, NULL, 0);
		parent_process(fileout, argv, envp, end);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	filein;
	int	fileout;

	if (argc == 5)
	{
		filein = open(argv[1], O_RDONLY, 0777);
		if (filein == -1)
		{
			perror("filein failed");
			exit(EXIT_FAILURE);
		}
		fileout = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fileout == -1)
		{
			perror("fileout failed");
			exit(EXIT_FAILURE);
		}
		pipex(filein, fileout, argv, envp);
	}
	else
		ft_putstr_fd("Missing arguments or files", 2);
	return (0);
}
