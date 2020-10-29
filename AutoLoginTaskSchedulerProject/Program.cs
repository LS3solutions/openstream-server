using System;
using System.Runtime.CompilerServices;
using Microsoft.Win32.TaskScheduler;

// This project is the one in charge 
// of the auto start mechanism usen by 
// Openstream server to start automatically
// When an user logs on.
// The scheduled task can run with All the 
// admin privileges.

namespace ScheduleTask
{
   class Program
    {
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                System.Console.WriteLine("[ScheduleTask] No argument received for task schedule. Error.");
                return;
            }

            String executablePath = args[0];
            String executableDir = args[1];

            System.Console.WriteLine($"[ScheduleTask] Path:{executablePath}\n[ScheduleTask] Dir:{executableDir}");

            if(TaskAlreadyExists("OpenStream autologon start"))
            {
                System.Console.WriteLine($"[ScheduleTask] Schedule task already set. Do nothing.");
                return;
            }
 
            // Create a new task definition for the local machine and assign properties
            TaskDefinition td = TaskService.Instance.NewTask();
            td.RegistrationInfo.Description = "Open Stream Server Task";

            //Allows the task to run with admin rights
            td.Principal.RunLevel = TaskRunLevel.Highest;

            //Allows the task to run infinetly. 
            td.Settings.ExecutionTimeLimit.Add(TimeSpan.Zero);

            LogonTrigger lt = new LogonTrigger();
            lt.Delay = TimeSpan.FromMilliseconds(1000 * 10); // V2 only


            td.Triggers.Add(lt);
            // Create an action that will launch Notepad whenever the trigger fires
            td.Actions.Add(executablePath, null, executableDir);
            // Register the task in the root folder of the local machine
            TaskService.Instance.RootFolder.RegisterTaskDefinition("OpenStream autologon start", td);
            System.Console.WriteLine($"[ScheduleTask] Schedule task created successfuly.");
        }

        static public bool TaskAlreadyExists(String tskName)
        {
            TaskService taskService = new TaskService();
            Task taskObject =  taskService.GetTask(tskName);
            if(taskObject is null)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
}
