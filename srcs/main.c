/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicoue <tnicoue@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 10:17:45 by tnicoue           #+#    #+#             */
/*   Updated: 2022/05/24 10:29:23 by tnicoue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	parent(int fileout, char **argv, char **envp, int *tuyau)
{
	dup2(tuyau[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(tuyau[1]);
	run(argv[3], envp);
}

void	child(int filein, char **argv, char **envp, int *tuyau)
{
	dup2(tuyau[1], STDOUT_FILENO);
	dup2(filein, STDIN_FILENO);
	close(tuyau[0]);
	run(argv[2], envp);
}

void	pipex(int filein, int fileout, char **argv, char **envp)
{
	int		tuyau[2];
	int		pid;

	if (pipe(tuyau) == -1)
	{
		perror("Pipe failed");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		child(filein, argv, envp, tuyau);
	}
	else
	{
		waitpid(pid, NULL, 0);
		parent(fileout, argv, envp, tuyau);
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
