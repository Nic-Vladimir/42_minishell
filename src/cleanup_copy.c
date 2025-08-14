/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_copy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgavornik <mgavornik@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 21:35:07 by vnicoles          #+#    #+#             */
/*   Updated: 2025/08/14 18:32:43 by mgavornik        ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/ast.h"
#include "../inc/env.h"
#include "../inc/minishell.h"
#include "../inc/tokenizer.h"

void	free_tokens(t_tokenizer_data *tokenizer)
{
	t_token	*current;
	t_token	*next;

	current = tokenizer->tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	tokenizer->tokens = NULL;
	tokenizer->tail = NULL;
}

void	free_ast(t_ast_node **node_ptr)
{
	int			i;
	t_ast_node	*node;

	if (!node_ptr || !*node_ptr)
		return ;
	node = *node_ptr;
	if (node->left)
		free_ast(&(node->left));
	if (node->right)
		free_ast(&(node->right));
	if (node->group)
		free_ast(&(node->group));
	if (node->args)
	{
		i = 0;
		while (node->args[i])
			free(node->args[i++]);
		free(node->args);
	}
	if (node->arg_types)
		free(node->arg_types);
	free(node);
	*node_ptr = NULL;
}

void	free_hashmap(t_hashmap *hashmap)
{
	t_bucket	*current;
	t_bucket	*next;
	ssize_t		i;

	if (!hashmap)
		return ;
	i = 0;
	while (i < hashmap->size)
	{
		current = hashmap->buckets[i];
		while (current)
		{
			next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			current = next;
		}
		i++;
	}
	free(hashmap->buckets);
	free(hashmap);
}
/*
static void	free_hashmap(t_hashmap *hashmap)
{
	t_bucket	*current;
	t_bucket	*next;

	if (!hashmap)
		return ;
	for (ssize_t i = 0; i < hashmap->size; i++)
	{
		current = hashmap->buckets[i];
		while (current)
		{
			next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			current = next;
		}
	}
	free(hashmap->buckets);
	free(hashmap);
}
*/

// void	free_env(t_env *env)
// {
// 	if (!env)
// 		return ;
// 	free_hashmap(env->vars);
// 	free_tokens(env->tokenizer);
// 	free(env->tokenizer);
// 	free(env->input);
// 	free(env);
// }

void	free_sig(t_sigenv *sigenv)
{
	if (!sigenv)
		return ;
	if (sigenv->def)
	{
		free(sigenv->def);
		sigenv->def = NULL;
	}
	sigenv->env = NULL;
	free(sigenv);
}

void	free_env(t_env *env)
{
	if (!env)
		return ;
	if (env->vars)
		free_hashmap(env->vars);
	if (env->tokenizer)
	{
		free_tokens(env->tokenizer);
		free(env->tokenizer);
	}
	if (env->sigenv)
	{
		if (env->sigenv->env && env->sigenv->env == env)
			env->sigenv->env = NULL;
		free_sig(env->sigenv);
	}
	if(env->pipeline)
	{
		free_pipeline_list(&env->pipeline);
	}
	if (env->input)
	{
		free(env->input);
		env->input = NULL;
	}
	free(env);
}


void	comprehensive_cleanup(t_env **env)
{
	if (!env || !*env)
		return;
	
	//fprintf(stderr, "[DEBUG] Starting comprehensive cleanup...\n");
	
	//Free AST structures
	if ((*env)->root)
	{
		//fprintf(stderr, "[DEBUG] Freeing AST...\n");
		free_ast(&((*env)->root));
		(*env)->root = NULL;
	}
	
	//Free tokenizer structures
	if ((*env)->tokenizer)
	{
		//fprintf(stderr, "[DEBUG] Freeing tokenizer...\n");
		free_tokens((*env)->tokenizer);
		free((*env)->tokenizer);
		(*env)->tokenizer = NULL;
	}
	
	//Free pipeline list
	if ((*env)->pipeline)
	{
		//fprintf(stderr, "[DEBUG] Freeing pipeline...\n");
		free_pipeline_list(&((*env)->pipeline));
		(*env)->pipeline = NULL;
	}
	
	//Free input string
	if ((*env)->input)
	{
		//fprintf(stderr, "[DEBUG] Freeing input string...\n");
		free((*env)->input);
		(*env)->input = NULL;
	}
	
	//Free hashmap
	if ((*env)->vars)
	{
		//fprintf(stderr, "[DEBUG] Freeing hashmap...\n");
		free_hashmap((*env)->vars);
		(*env)->vars = NULL;
	}
	
	//Free signal environment
	if ((*env)->sigenv)
	{
		//fprintf(stderr, "[DEBUG] Freeing signal environment...\n");
		if ((*env)->sigenv->env && (*env)->sigenv->env == *env)
			(*env)->sigenv->env = NULL;
		free_sig((*env)->sigenv);
		(*env)->sigenv = NULL;
	}
	
	//Free the main env structure
	//fprintf(stderr, "[DEBUG] Freeing main env structure...\n");
	free(*env);
	*env = NULL;
	
	//Clean up readline
	//fprintf(stderr, "[DEBUG] Cleaning readline...\n");
	clean_rl();
	
	//fprintf(stderr, "[DEBUG] Comprehensive cleanup completed.\n");
}


void	child_comprehensive_cleanup(t_env *env)
{
	report_memory_usage(env);
	if (!env)
		return;
		
	//fprintf(stderr, "[DEBUG] Child %d: Starting inherited structure cleanup...\n", getpid());
	
       if (env->root)
       {
	       free_ast(&(env->root));
	       env->root = NULL;
       }
       if (env->pipeline)
       {
	       free_pipeline_list(&(env->pipeline));
	       env->pipeline = NULL;
       }

       if (env->tokenizer)
       {
	    	clear_tok_data(env->tokenizer);
	    	//env->tokenizer = NULL;

       }

       if (env->input)
       {
	       free(env->input);
	       env->input = NULL;
       }

	
		
	//fprintf(stderr, "[DEBUG] Child %d: Inherited structure cleanup completed.\n", getpid());
}

/**
 * Debug function to report memory usage statistics
 * Call this before comprehensive_cleanup to see what needs freeing
 */
void	report_memory_usage(t_env *env)
{
	int count = 0;
	
	if (!env)
	{
		//fprintf(stderr, "[MEMORY] env is NULL\n");
		return;
	}
	
	//fprintf(stderr, "[MEMORY] === Memory Usage Report ===\n");
	//fprintf(stderr, "[MEMORY] env pointer: %p\n", (void*)env);
	
	if (env->vars)
	{
		//fprintf(stderr, "[MEMORY] hashmap: %p (ALLOCATED)\n", (void*)env->vars);
		count++;
	}
	else
	{
		;//fprintf(stderr, "[MEMORY] hashmap: NULL\n");
	}
	if (env->tokenizer)
	{
		////fprintf(stderr, "[MEMORY] tokenizer: %p (ALLOCATED)\n", (void*)env->tokenizer);
		count++;
	}
	else
		//fprintf(stderr, "[MEMORY] tokenizer: NULL\n");
		
	if (env->root)
	{
		//fprintf(stderr, "[MEMORY] AST root: %p (ALLOCATED)\n", (void*)env->root);
		count++;
	}
	else
	{
		;//fprintf(stderr, "[MEMORY] AST root: NULL\n");
	}	
	if (env->pipeline)
	{
		//fprintf(stderr, "[MEMORY] pipeline: %p (ALLOCATED)\n", (void*)env->pipeline);
		count++;
	}
	else
	{
		;//fprintf(stderr, "[MEMORY] pipeline: NULL\n");
	}
	if (env->input)
	{
		//fprintf(stderr, "[MEMORY] input: %p -> \"%s\" (ALLOCATED)\n", (void*)env->input, env->input);
		count++;
	}
	else
	{
		;//fprintf(stderr, "[MEMORY] input: NULL\n");
	}
	if (env->sigenv)
	{
		//fprintf(stderr, "[MEMORY] sigenv: %p (ALLOCATED)\n", (void*)env->sigenv);
		count++;
	}
	else
	{
		;//fprintf(stderr, "[MEMORY] sigenv: NULL\n");
	}
	//fprintf(stderr, "[MEMORY] Total allocated structures: %d\n", count);
	//fprintf(stderr, "[MEMORY] === End Report ===\n");
}

/**
 * Force cleanup of all known memory structures across the entire project
 * This is a "nuclear option" for complete memory cleanup
 */
void	force_cleanup_all_memory(t_env **env)
{
	if (!env || !*env)
	{
		//fprintf(stderr, "[FORCE_CLEANUP] Nothing to clean up - env is NULL\n");
		return;
	}
	
	//fprintf(stderr, "[FORCE_CLEANUP] Starting FORCE cleanup of all memory...\n");
	
	report_memory_usage(*env);
	comprehensive_cleanup(env);
	rl_clear_history();
	//fprintf(stderr, "[FORCE_CLEANUP] FORCE cleanup completed.\n");
}

void	free_everything(t_env **env)
{
	if (!env)
	{
		//fprintf(stderr, "[FREE_EVERYTHING] Warning: env parameter is NULL\n");
		return;
	}
	
	//fprintf(stderr, "[FREE_EVERYTHING] Freeing all malloc'd structures...\n");
	force_cleanup_all_memory(env);
	//fprintf(stderr, "[FREE_EVERYTHING] All memory freed.\n");
}
