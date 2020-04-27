namespace Interface.Tools.Navigation
{
    internal enum ViewType
    {
        EngineControl,
        UserInput
    }

    interface INavigationModel
    {
        void Navigate(ViewType viewType);
    }
}
