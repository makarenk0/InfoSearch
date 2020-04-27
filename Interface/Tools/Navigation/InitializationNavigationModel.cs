using System;
using EngineControlView = Interface.View.EngineControlView;
using UserInputView = Interface.View.UserInputView;

namespace Interface.Tools.Navigation
{
    internal class InitializationNavigationModel : BaseNavigationModel
    {
        public InitializationNavigationModel(IContentOwner contentOwner) : base(contentOwner)
        {
            
        }
   
        protected override void InitializeView(ViewType viewType)
        {
            switch (viewType)
            {
                case ViewType.EngineControl:
                    ViewsDictionary.Add(viewType, new EngineControlView());
                    break;
                case ViewType.UserInput:
                    ViewsDictionary.Add(viewType, new UserInputView());
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(viewType), viewType, null);
            }

        }
    }
}
