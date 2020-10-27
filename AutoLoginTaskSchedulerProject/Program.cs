using System;
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

            // Create a new task definition for the local machine and assign properties
            TaskDefinition td = TaskService.Instance.NewTask();
            td.RegistrationInfo.Description = "Open Stream Server Task";

            //Allows the task to run with admin rights
            td.Principal.RunLevel = TaskRunLevel.Highest;

            //Allows the task to run infinetly. 
            td.Settings.ExecutionTimeLimit.Add(TimeSpan.Zero);

            LogonTrigger lt = new LogonTrigger();
            lt.Delay = TimeSpan.FromMinutes(1); // V2 only


            td.Triggers.Add(lt);
            // Create an action that will launch Notepad whenever the trigger fires
            td.Actions.Add(executablePath, null, executableDir);
            // Register the task in the root folder of the local machine
            TaskService.Instance.RootFolder.RegisterTaskDefinition("OpenStream autologon start", td);
        }
    }
}
