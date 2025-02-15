<?php

namespace App\Form;

use App\Entity\Employee;
use App\Entity\Role;
use Symfony\Bridge\Doctrine\Form\Type\EntityType;
use Symfony\Component\Form\AbstractType;
use Symfony\Component\Form\Extension\Core\Type\EmailType;
use Symfony\Component\Form\Extension\Core\Type\TextType;
use Symfony\Component\Form\Extension\Core\Type\SubmitType;
use Symfony\Component\Form\FormBuilderInterface;
use Symfony\Component\OptionsResolver\OptionsResolver;
use Symfony\Component\Validator\Constraints\NotBlank;

class CreateEmployeeType extends AbstractType
{
    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        $isEdit = $options['is_edit'] ?? false;

        $builder
            ->add('firstName', TextType::class, [
                'label' => 'Jméno',
                'constraints' => [new NotBlank()],
            ])
            ->add('lastName', TextType::class, [
                'label' => 'Příjmení',
                'constraints' => [new NotBlank()],
            ])
            ->add('roles', EntityType::class, [
                'class' => Role::class,
                'choice_label' => 'title',
                'label' => 'Funkce',
                'multiple' => true,
                'expanded' => true,
                'constraints' => [
                    new NotBlank(),
                ],
                'attr' => ['class' => 'custom-block'],
            ])
            ->add('email', EmailType::class, [
                'label' => 'Email',
                'constraints' => [new NotBlank()],
            ])
            ->add('phone', TextType::class, [
                'label' => 'Telefon',
                'constraints' => [new NotBlank()],
            ])
            ->add('website', TextType::class, [
                'label' => 'Webová stránka',
                'required' => false,
            ])
            ->add('description', TextType::class, [
                'label' => 'Popis',
                'required' => false,
            ])
            ->add('submit', SubmitType::class, [
                'label' => $isEdit ? 'Úpravit zaměstnance' : 'Vytvořit zaměstnance',
                'attr' => ['class' => 'btn-primary'],
            ]);
    }

    public function configureOptions(OptionsResolver $resolver)
    {
        $resolver->setDefaults([
            'data_class' => Employee::class,
            'is_edit' => false,
        ]);
    }
}

