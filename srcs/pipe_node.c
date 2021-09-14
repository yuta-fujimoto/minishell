static bool	exit_failure(char *message)
{
	//print message to stderr
	return (false);	
}

//bool	pipe_node(t_node l, t_node r)
int	main(void)
{
	int		fd1[2];
	int		fd2[2];
	pid_t	c_pid;

	if (pipe(fd1) == -1 || pipe(fd2) == -1)
		return (exit_failure(“mssg”));
//	c_pid = fork();
//	if (c_pid < 0) 
	printf(“%d%d%d%d\n”, fd1[0], fd1[1], fd2[0], fd2[1]);
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
}
