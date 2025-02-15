<?php


namespace App\Form;

use App\Entity\Role;
use Symfony\Component\Form\AbstractType;
use Symfony\Component\Form\Extension\Core\Type\TextType;
use Symfony\Component\Form\Extension\Core\Type\TextareaType;
use Symfony\Component\Form\Extension\Core\Type\CheckboxType;
use Symfony\Component\Form\Extension\Core\Type\SubmitType;
use Symfony\Component\Form\FormBuilderInterface;
use Symfony\Component\OptionsResolver\OptionsResolver;
use Symfony\Component\Validator\Constraints\NotBlank;

class CreateRoleType extends AbstractType
{
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $isEdit = $options['is_edit'] ?? false;

        $builder
            ->add('title', TextType::class, [
                'label' => 'Název role',
                'constraints' => [new NotBlank()],
            ])
            ->add('description', TextType::class, [
                'label' => 'Popis',
                'required' => false,
            ])
            ->add('isVisible', CheckboxType::class, [
                'label' => 'Viditelná role',
                'required' => false,
                'attr' => ['class' => 'custom-block'],
            ])
            ->add('submit', SubmitType::class, [
                'label' => $isEdit ? 'Upravit roli' : 'Vytvořit roli',
                'attr' => ['class' => 'btn-primary'],
            ]);
    }

    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults([
            'data_class' => Role::class,
            'is_edit' => false,
        ]);
    }
}
